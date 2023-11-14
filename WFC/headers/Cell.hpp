#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#pragma once

class Cell
{
private:
	int collapsedState;			 // cell's final state
	int coords;					 // coordinates in tile map where cell is
	std::vector<int> possStates; // Vector holding possible tile indicies and the number of directions the tile is enabled in
	float sumTileWeights;		 // sum of the tile weights, for use finding entropy
	float logSumTileWeights;	 // log sum of the tile weights, for use finding entropy

public:
	Cell(int numStates, int coords, float sumTileWeights, float logSumTileWeights);
	bool operator>(const Cell &rhs) const;		 // Operator for comparing entropy
	bool operator==(const Cell &rhs) const;		 // Operator for comparing entropy
	void elim(int index, int tileFreq);			 // eliminates the possibility of tile at that index
	bool removeEnabler(int index, int tileFreq); // Remove enabler from tile index, update tile frequency if truly eliminated
	int collapse(std::vector<int> &freqRules);	 // Collapse cell
	int getCollapsedState();					 // get collapsed state
	int getCoords();							 // get coordinates in tilemap
	void updateTileWeights(int tileFreq);		 // update tile weights

	bool getPossTile(int index); // return if tile is a possibility
	int getPossStateSize();		 // get the size of possible tiles
	float getEntropy() const;	 // return entropy
};
