#include <iostream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "InputManager.hpp"
#include "Direction.hpp"
#pragma once

class Rules
{
private:
	std::map<int, std::vector<std::pair<int, DIRECTION>>> adjRules; // adjacency rules (What tiles can be next to what tiles in which direction)
	std::vector<int> freqRules;										// Frequency rules about how often each tile should appear
public:
	Rules(const sf::Vector2u tileSize, const std::string &filePath);
	~Rules();

	int getAdjSize();														// return adjacency rule size
	int getFreqSize();														// return frequency rule size
	int getFreqRule(int index);												// return a specific freqency rule
	std::vector<int> &getFreqRules();										// Gets a frequency rule for a specific UNIQUE tile (need unique tile index)
	std::vector<int> getAdjRule(int index, DIRECTION dir);					// Gets adj rules for specific directions
	bool isEnabled(int mainTileIndex, int suspectTileIndex, DIRECTION dir); // does the main tile enable the suspect tile in "dir" direction?
	bool hasDir(int index, DIRECTION dir);									// is the tile neutral in that direction?
	void printAdjRules();													// print rules (for debuggin)
};
