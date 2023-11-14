#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <stack>
#include "Rules.hpp"
#include "Cell.hpp"
#include "RemovalCard.hpp"
#pragma once

// Struct to compare entropy heap
// Must dereference cells to get entropy
struct PtrGreaterThan
{
	bool operator()(const Cell *lhs, const Cell *rhs) const
	{
		return *lhs > *rhs;
	}
};

class Grid
{
private:
	Rules *rules;							// holds adjacency and frequency rules
	std::vector<Cell *> entropyHeap;		// min heap that organizes cells based on entropy (least possible choices)
	std::stack<RemovalCard *> removalStack; // stack that holds cells with recently eliminated possibilities, for use to propogate implications throughout grid
	sf::Vector2u dims;						// dimensions of wanted tile map
	std::vector<Cell *> workingTM;			// pointer to an array that contains the cells with their possible states
	int *finalTM;							// pointer to an array that contains the final tilemap, index of unique tiles

	bool initGrid();																 // collapse grid
	void createWorkingTM();															 // fill working TM with new cells
	void clearWorkingTM();															 // delete memroy from working TM
	void popEntropyHeap();															 // remove cell with least entropy
	int cellAt(int i, int j);														 // get cell at coords
	void createFinal();																 // copy working tm over to final tm
	sf::Vector2f calculateTileWeights();											 // calcualte initial tileweights for cells
	void checkNeighbors(int coords);												 // check cells for unenabled tiles
	bool removeAfterCollapse(int coords);											 // remove contradictions in neighboring cells
	void removeContradictions(int collapsedTileIndex, int cellIndex, DIRECTION dir); // Used for collapsed cells
	void removeEnablers(int coords, int cellIndex, DIRECTION dir);					 // Used for uncollapsed cells, or removal stack (pushed single tile at a time)

public:
	Grid(const sf::Vector2u dims, const sf::Vector2u tileSize, const std::string &filePath);
	~Grid()
	{
		while (!removalStack.empty())
		{
			delete removalStack.top();
			removalStack.pop();
		}
		if (finalTM != nullptr)
			delete[] finalTM;
		for (int i = 0; i < workingTM.size(); i++)
		{
			delete workingTM[i];
		}
		workingTM.clear();
		entropyHeap.clear();
		delete rules;
	};

	int *getFinal(); // get final tile map
};