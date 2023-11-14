#include "../headers/InputManager.hpp"

InputManager::InputManager(sf::Vector2u tileSize)
{
	this->tileSize = tileSize;
}

// Check if adjacency rule pairing already exists for a tile
bool InputManager::pairExists(int first, DIRECTION second, std::vector<std::pair<int, DIRECTION>> &adjacent)
{
	for (int i = 0; i < adjacent.size(); i++)
		if (adjacent[i].first == first && adjacent[i].second == second)
			return true;
	return false;
}

// Using tile coords, get the correct coords in an image array
int InputManager::getCoords(int coordx, int coordy)
{
	// first argument: Find column position, tile width in pixels * column num based on tile.
	// Second argument: Find row position, tile height in pixes * row num based on tile * image width to move down by
	return 4 * ((coordy * tileSize.x) + (coordx * tileSize.y * imgSize.x));
}

// Export unique tiles in one image
void InputManager::exportImg(sf::Uint8 *newPix, bool replaceOld, sf::Vector2u imageSize)
{
	sf::Image newImg;
	std::string fileName;
	if (!replaceOld)
		fileName = "../../Textures/outputImg" + std::to_string(rand()) + ".png";
	else
		fileName = "../../Textures/UniqueTiles.png";
	newImg.create(imageSize.x, imageSize.y, newPix);
	if (newImg.saveToFile(fileName))
		std::cout << "Saved Image" << std::endl;
}

// Takes in two pointers to variables and iterates through a tile size of pixels
// if the tiles differ in ONE pixel, then they are not the same
bool InputManager::compareTiles(const sf::Uint8 *curTile, const sf::Uint8 *rhs)
{
	for (int x = 0; x < tileSize.x; x++)
	{
		for (int y = 0; y < tileSize.y; y++)
		{

			if (curTile[4 * (x * imgSize.x + y)] != rhs[4 * (x * imgSize.x + y)] ||
				curTile[4 * (x * imgSize.x + y) + 1] != rhs[4 * (x * imgSize.x + y) + 1] ||
				curTile[4 * (x * imgSize.x + y) + 2] != rhs[4 * (x * imgSize.x + y) + 2] ||
				curTile[4 * (x * imgSize.x + y) + 3] != rhs[4 * (x * imgSize.x + y) + 3])
			{
				return false;
			}
		}
	}
	return true;
}

// Copy tile pixel by pixel into an array starting at "containerStartIt"
int InputManager::copyTile(const sf::Uint8 *curTile, sf::Uint8 *container, int containerStartIt)
{
	for (int x = 0; x < tileSize.x; x++)
	{
		for (int y = 0; y < tileSize.y; y++)
		{
			container[containerStartIt] = curTile[4 * (x * imgSize.x + y)];
			container[containerStartIt + 1] = curTile[4 * (x * imgSize.x + y) + 1];
			container[containerStartIt + 2] = curTile[4 * (x * imgSize.x + y) + 2];
			container[containerStartIt + 3] = curTile[4 * (x * imgSize.x + y) + 3];
			containerStartIt += 4;
		}
	}

	return containerStartIt;
}

// load image and separate out unique tiles
bool InputManager::loadSampleImage(const std::string &imgPath)
{
	sf::Image img;
	if (!img.loadFromFile(imgPath))
		return false;

	this->imgSize = img.getSize();
	this->tileDims = sf::Vector2u(imgSize.x / tileSize.x, imgSize.y / tileSize.y);
	tileMap = new int[tileDims.x * tileDims.y];

	const sf::Uint8 *pxlPtr = img.getPixelsPtr();
	std::vector<sf::Vector2u> uniqTileCoords;
	uniqTileCoords.push_back(sf::Vector2u(0, 0));
	std::string fileName;

	//   Iterate through the pixels for each tile in image
	for (int i = 0; i < tileDims.y; i++)
	{
		for (int j = 0; j < tileDims.x; j++)
		{

			// Compare current tile with tiles in the vector
			for (int k = 0; k < uniqTileCoords.size(); k++)
			{
				// If currtile matches, then break, tile isn't unique
				if (compareTiles(&pxlPtr[getCoords(i, j)], &pxlPtr[getCoords(uniqTileCoords[k].x, uniqTileCoords[k].y)]))
				{
					tileMap[i * tileDims.x + j] = k;
					break;
				}
				else if (k == uniqTileCoords.size() - 1) // everything has been checked by this point, add to vector
				{
					tileMap[i * tileDims.x + j] = uniqTileCoords.size();
					uniqTileCoords.push_back(sf::Vector2u(i, j));
				}
			}
		}
	}

	uniqueTileCount = uniqTileCoords.size();

	//  Collect all of the unique tiles into one.
	int fTSIter = 0;
	sf::Uint8 *finalTileSet = new sf::Uint8[tileSize.x * tileSize.y * 4 * uniqTileCoords.size()];
	for (int k = 0; k < uniqueTileCount; k++)
	{
		fTSIter = copyTile(&pxlPtr[getCoords(uniqTileCoords[k].x, uniqTileCoords[k].y)], finalTileSet, fTSIter);
	}
	exportImg(finalTileSet, true, sf::Vector2u(tileSize.x, tileSize.y * uniqueTileCount));
	delete[] finalTileSet;

	return true;
};

// Create adjacency rules based on what tiles are next to each other in sample image
std::map<int, std::vector<std::pair<int, DIRECTION>>> InputManager::createAdjRules()
{
	// Create an adjacency matrix
	// Go through the tile map, and check directions
	// For each tile in each direction, add the direction to the adjacency matrix

	std::map<int, std::vector<std::pair<int, DIRECTION>>> adjRules;

	// initialize Unique Tile keys
	for (int i = 0; i < uniqueTileCount; i++)
	{
		adjRules[i];
	}

	for (int i = 0; i < tileDims.y; i++)
	{
		for (int j = 0; j < tileDims.x; j++)
		{
			int tmCoord = j + i * tileDims.x;
			int tmCoordCheck;
			if (tmCoord - (int)tileDims.x > -1) // Get Up
			{
				tmCoordCheck = tmCoord - tileDims.x;
				if (!pairExists(tileMap[tmCoordCheck], UP, adjRules[tileMap[tmCoord]]))
					adjRules[tileMap[tmCoord]].push_back(std::pair(tileMap[tmCoordCheck], UP));
			}
			if (tmCoord + (int)tileDims.x < tileDims.x * tileDims.y) // Get Down
			{
				tmCoordCheck = tmCoord + tileDims.x;
				if (!pairExists(tileMap[tmCoordCheck], DOWN, adjRules[tileMap[tmCoord]]))
					adjRules[tileMap[tmCoord]].push_back(std::pair(tileMap[tmCoordCheck], DOWN));
			}
			if (tmCoord - 1 > -1 && tmCoord < tileDims.x * tileDims.y && tmCoord % (int)tileDims.x != 0) // Get Left
			{
				tmCoordCheck = tmCoord - 1;
				if (!pairExists(tileMap[tmCoordCheck], LEFT, adjRules[tileMap[tmCoord]]))
					adjRules[tileMap[tmCoord]].push_back(std::pair(tileMap[tmCoordCheck], LEFT));
			}
			if (tmCoord + 1 < tileDims.x * tileDims.y && (tmCoord + 1) % (int)tileDims.x != 0) // Get Right
			{
				tmCoordCheck = tmCoord + 1;
				if (!pairExists(tileMap[tmCoordCheck], RIGHT, adjRules[tileMap[tmCoord]]))
					adjRules[tileMap[tmCoord]].push_back(std::pair(tileMap[tmCoordCheck], RIGHT));
			}
		}
	}

	return adjRules;
}

// Create frequency rules based on how many times a tile appears
std::vector<int> InputManager::createFreqRules()
{
	// create freqRules
	// Iterate through sample map (tileMap)
	// count the number of times each tile appears

	std::vector<int> freqRules;

	// initilaize tile indices
	for (int i = 0; i < uniqueTileCount; i++)
	{
		freqRules.push_back(0);
	}

	for (int i = 0; i < tileDims.y; i++)
	{
		for (int j = 0; j < tileDims.x; j++)
		{
			int tmCoord = j + i * tileDims.x;
			freqRules[tileMap[tmCoord]] += 1;
		}
	}

	return freqRules;
}