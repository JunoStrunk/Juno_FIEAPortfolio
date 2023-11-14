#include <iostream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Direction.hpp"
#pragma once

class InputManager
{
private:
	sf::Vector2u tileDims; // size of image in tiles (width and height by tile)
	sf::Vector2u tileSize; // tile size in pixels
	sf::Vector2u imgSize;  // image size in pixels
	int uniqueTileCount;   // Number of unique tiles
	int *tileMap;		   // Pointer to an array that contains the sample image translated into the index of the unique tile

	bool pairExists(int first, DIRECTION second, std::vector<std::pair<int, DIRECTION>> &adjacent); // check if adjacency pairing exists for a tile already
	int getCoords(int coordx, int coordy);															// get coords in image array
	void exportImg(sf::Uint8 *newPix, bool replaceOld, sf::Vector2u imageSize);						// export tiles in one image
	bool compareTiles(const sf::Uint8 *curTile, const sf::Uint8 *rhs);								// compare two tiles, if they are the same, true
	int copyTile(const sf::Uint8 *curTile, sf::Uint8 *container, int containerStartIt);				// copy tile pixels into an array

public:
	InputManager(const sf::Vector2u tileSize);
	~InputManager()
	{
		delete[] tileMap;
	};

	bool loadSampleImage(const std::string &imgPath);						// load sample image, separate out unique tiles
	std::map<int, std::vector<std::pair<int, DIRECTION>>> createAdjRules(); // create adjacency rules
	std::vector<int> createFreqRules();										// create freqeuency rules
};
