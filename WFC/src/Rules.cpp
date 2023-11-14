#include "../headers/Rules.hpp"

Rules::Rules(const sf::Vector2u tileSize, const std::string &filePath)
{
	InputManager input(tileSize);
	if (!input.loadSampleImage(filePath))
		throw "Unable to Load Sample Image";

	adjRules = input.createAdjRules();
	freqRules = input.createFreqRules();
}

Rules::~Rules()
{
}

int Rules::getAdjSize()
{
	return adjRules.size();
}

int Rules::getFreqSize()
{
	return freqRules.size();
}

int Rules::getFreqRule(int index)
{
	return freqRules[index];
}

std::vector<int> &Rules::getFreqRules()
{
	return freqRules;
}

// get possible tiles in a specific direction from specific tile
std::vector<int> Rules::getAdjRule(int index, DIRECTION dir)
{
	std::vector<int> possInDir;
	for (int i = 0; i < adjRules.at(index).size(); i++)
	{
		if (adjRules.at(index).at(i).second == dir)
			possInDir.push_back(adjRules.at(index).at(i).first);
	}

	// return false;
	return possInDir;
}

// Cycles through adjRules of adjacent tile, compares them to one possible state of the uncollapsed tile
// to see if that state is still supported from that direction
// Direction is OPPOSITE to uncollapsedTile
bool Rules::isEnabled(int mainTileIndex, int suspectTileIndex, DIRECTION dir) // is present
{

	for (int i = 0; i < adjRules.at(mainTileIndex).size(); i++) // go through mainTiles's adjrules
	{
		if (adjRules.at(mainTileIndex).at(i).first == suspectTileIndex && adjRules.at(mainTileIndex).at(i).second == dir)
		{
			return true; // is enabled from the direction
		}
	}
	return false; // isn't enabled from the direction
}

// if tile has no rules in that direction, return false
bool Rules::hasDir(int index, DIRECTION dir)
{
	for (int i = 0; i < adjRules.at(index).size(); i++)
	{
		if (adjRules.at(index).at(i).second == dir)
			return true;
	}

	return false;
}

void Rules::printAdjRules()
{
	std::map<int, std::vector<std::pair<int, DIRECTION>>>::iterator it = adjRules.begin();
	while (it != adjRules.end())
	{
		std::cout << it->first << ": ";
		for (int i = 0; i < it->second.size(); i++)
		{
			std::cout << "(" << it->second[i].first << ", " << it->second[i].second << ")";
		}
		std::cout << "\n";
		it++;
	}
}
