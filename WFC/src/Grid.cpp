#include "../headers/Grid.hpp"

Grid::Grid(const sf::Vector2u dims, const sf::Vector2u tileSize, const std::string &filePath)
{
	rules = new Rules(tileSize, filePath); // create ruleset
	this->dims = dims;
	finalTM = new int[dims.x * dims.y];

	createWorkingTM();

	int tryCount = 0;		   // keep track of how many tries are taken. In case we want a cutoff point
	bool success = initGrid(); // try to collapse grid
	while (!success)		   // if unsuccessful retry until successful
	{
		std::cout << "Retrying " << tryCount << "\n";
		clearWorkingTM();
		createWorkingTM();
		success = initGrid();
		tryCount++;
	}

	std::cout << "Had to retry " << tryCount << " times." << std::endl;
}

bool Grid::initGrid()
{
	int numCollapsedCells = 0;					// keep track of total number of collapsed cells
	while (numCollapsedCells < dims.x * dims.y) // while there are stills cells to be collapsed
	{
		std::make_heap(entropyHeap.begin(), entropyHeap.end(), PtrGreaterThan()); // Sort the entropyHeap vector into a minheap O(n)

		// if the first entry is collapsed, remove from heap
		// keep doing this until one isn't collapsed
		while (entropyHeap[0]->getCollapsedState() != -1)
		{
			popEntropyHeap();
		}

		// collapse first cell in entropy heap, if it cannot be collapsed (Contradiction)
		// return false.
		if (entropyHeap[0]->collapse(rules->getFreqRules()) == -1)
			return false;

		// Output to console how far along we are in collapsing
		std::cout << "Collapsed " << numCollapsedCells << "/" << (dims.x * dims.y) << std::endl;

		// remove any contradictions in initial neighbors
		removeAfterCollapse(entropyHeap[0]->getCoords());

		numCollapsedCells++;

		while (!(removalStack.empty()))
		{
			// get info for top card and pop it before checking neighbors
			// stored in a stack, so we need to remove the one we're checking before
			// adding any others.
			int removedCoords = removalStack.top()->getRemovedCoords();
			delete removalStack.top();
			removalStack.pop();
			checkNeighbors(removedCoords);
		}

		// take the cell we just collapsed and pop it from the entropy heap.
		popEntropyHeap();
	}

	std::cout << "All cells collapsed\n";
	createFinal();
	return true;
}

// Create working TM
// fill with new cells
// fill entropy heap with new cells
void Grid::createWorkingTM()
{
	sf::Vector2f tileSums = calculateTileWeights();

	for (int i = 0; i < dims.y; i++)
	{
		for (int j = 0; j < dims.x; j++)
		{
			workingTM.push_back(new Cell(rules->getAdjSize(), cellAt(i, j), tileSums.x, tileSums.y));
			// fill entropy heap
			entropyHeap.push_back(workingTM[cellAt(i, j)]);
		}
	}

	// sort entropy heap vector into a MinHeap
	std::make_heap(entropyHeap.begin(), entropyHeap.end(), PtrGreaterThan());
}

// Delete memory from working TM
// for use when contradictions make tile set "Unsolvable"
// (Collapsed into a corner)
void Grid::clearWorkingTM()
{
	while (!removalStack.empty())
	{
		delete removalStack.top();
		removalStack.pop();
	}
	for (int i = 0; i < workingTM.size(); i++)
	{
		delete workingTM[i];
	}
	workingTM.clear();
	entropyHeap.clear();
}

// Pop from the entropy heap
void Grid::popEntropyHeap()
{
	pop_heap(entropyHeap.begin(), entropyHeap.end(), PtrGreaterThan());
	entropyHeap.pop_back();
}

// From x and y coords, calculate the coords for a single vector
int Grid::cellAt(int i, int j)
{
	return j + i * dims.x;
}

// Copy over the collapsed cells from working tile map, to final tile map
void Grid::createFinal()
{

	for (int i = 0; i < dims.y; i++)
	{
		for (int j = 0; j < dims.x; j++)
		{
			finalTM[cellAt(i, j)] = workingTM[cellAt(i, j)]->getCollapsedState();
		}
	}
}

// calcualte the initial tile weights
sf::Vector2f Grid::calculateTileWeights()
{
	float sumTileWeights = 0;
	float logSumTileWeights = 0;

	for (int i = 0; i < rules->getFreqSize(); i++)
	{
		sumTileWeights += rules->getFreqRule(i);
		logSumTileWeights += rules->getFreqRule(i) * log2(rules->getFreqRule(i));
	}

	return sf::Vector2f(sumTileWeights, logSumTileWeights);
}

// check neighboring cells to see if possible tiles still enable neighboring tiles
// for use with UNCOLLAPSED cells
void Grid::checkNeighbors(int coords)
{
	// Check up
	int cellIndex = coords - (int)dims.x;
	if (cellIndex > -1 && cellIndex < dims.y * dims.x)
	{
		removeEnablers(coords, cellIndex, UP);
	}
	// check down
	cellIndex = coords + (int)dims.x;
	if (cellIndex > -1 && cellIndex < dims.y * dims.x)
	{
		removeEnablers(coords, cellIndex, DOWN);
	}
	// check left
	cellIndex = coords - 1;
	if (cellIndex > -1 && cellIndex < dims.y * dims.x && coords % dims.x != 0)
	{
		removeEnablers(coords, cellIndex, LEFT);
	}
	// check right
	cellIndex = coords + 1;
	if (cellIndex > -1 && cellIndex < dims.y * dims.x && cellIndex % dims.x != 0)
	{
		removeEnablers(coords, cellIndex, RIGHT);
	}
}

// check neighbors to remove contradicting possibilities
// for use with COLLAPSED cells
bool Grid::removeAfterCollapse(int coords)
{
	int collapsedTileIndex = workingTM[coords]->getCollapsedState();
	if (collapsedTileIndex == -1)
		return false;
	// Check up
	int cellIndex = coords - (int)dims.x;
	if (rules->hasDir(collapsedTileIndex, UP) && cellIndex > -1 && cellIndex < dims.y * dims.x)
	{
		removeContradictions(collapsedTileIndex, cellIndex, UP);
	}
	// check down
	cellIndex = coords + (int)dims.x;
	if (rules->hasDir(collapsedTileIndex, DOWN) && cellIndex > -1 && cellIndex < dims.y * dims.x)
	{
		removeContradictions(collapsedTileIndex, cellIndex, DOWN);
	}
	// check left
	cellIndex = coords - 1;
	if (rules->hasDir(collapsedTileIndex, LEFT) && cellIndex > -1 && cellIndex < dims.y * dims.x && coords % dims.x != 0)
	{
		removeContradictions(collapsedTileIndex, cellIndex, LEFT);
	}
	// check right
	cellIndex = coords + 1;
	if (rules->hasDir(collapsedTileIndex, RIGHT) && cellIndex > -1 && cellIndex < dims.y * dims.x && cellIndex % dims.x != 0)
	{
		removeContradictions(collapsedTileIndex, cellIndex, RIGHT);
	}
	return true;
}

// Removes contradicitons from neighbors, when tile has been COLLAPSED
void Grid::removeContradictions(int collapsedTileIndex, int cellIndex, DIRECTION dir)
{
	// iterate through possible states in adj cell being checked
	for (int i = 0; i < workingTM[cellIndex]->getPossStateSize(); i++)
	{
		// for each possible tile in neighbor,
		// check if the collapsed tile allows it,
		// if it DOESN"T
		// eliminate that option from neightbor, and add to removal stack to have it's neighbors checked
		if (workingTM[cellIndex]->getPossTile(i) && !(rules->isEnabled(collapsedTileIndex, i, dir)))
		{
			workingTM[cellIndex]->elim(i, rules->getFreqRule(i));
			removalStack.push(new RemovalCard(i, cellIndex));
		}
	}
}

// Removes contradictions from neighbors, when tile HASN'T collapsed
void Grid::removeEnablers(int coords, int cellIndex, DIRECTION dir)
{
	bool enabledFromDir = false; // bool to check if the tile is enabled from that direction
	for (int i = 0; i < workingTM[cellIndex]->getPossStateSize(); i++)
	{
		for (int j = 0; j < workingTM[coords]->getPossStateSize(); j++)
		{
			// for each possible tile in neighbor,
			// and for each possible tile in curr cell
			// check if the curr tile enables the neighbor tile
			// if it does, no need to check anymore, no enablers need to be removed
			// set enabled from dir to true.
			if (workingTM[coords]->getPossTile(j) && workingTM[cellIndex]->getPossTile(i) && (rules->isEnabled(j, i, dir)))
			{
				enabledFromDir = true;
				break;
			}
		}

		// if no tiles in curr cell enable neighboring tile
		// remove an enabler from neighboring tile
		// if it was a true elimination (meaning no one in the four directions enable it)
		// push to removal stack to check neighbors
		if (!enabledFromDir)
		{
			bool trueElim = workingTM[cellIndex]->removeEnabler(i, rules->getFreqRule(i));
			if (trueElim) // if last enabler removed
			{
				removalStack.push(new RemovalCard(i, cellIndex));
			}
		}

		// reset enabled from direction for next possible tile
		enabledFromDir = false;
	}
}

// return final tile map
int *Grid::getFinal()
{
	return finalTM;
}