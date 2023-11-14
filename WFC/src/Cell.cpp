#include "../headers/Cell.hpp"

Cell::Cell(int numStates, int coords, float sumTileWeights, float logSumTileWeights)
{
	this->coords = coords;
	this->sumTileWeights = sumTileWeights;
	this->logSumTileWeights = logSumTileWeights;
	collapsedState = -1; // not collapsed
	for (int i = 0; i < numStates; i++)
	{
		possStates.push_back(4); // 4 directions a tile can be enabled from
	}
}

bool Cell::operator>(const Cell &rhs) const
{
	return this->getEntropy() > rhs.getEntropy();
}

bool Cell::operator==(const Cell &rhs) const
{
	return this->getEntropy() == rhs.getEntropy();
}

// eliminate possible state, update tile weights
void Cell::elim(int index, int tileFreq)
{
	possStates[index] = 0;
	updateTileWeights(tileFreq);
}

// remove enabler
bool Cell::removeEnabler(int index, int tileFreq)
{
	possStates[index]--;
	if (possStates[index] == 0) // if last enabler removed
	{
		elim(index, tileFreq); // update tile frequencies
		return true;		   //"True elimination" push to removal stack
	}
	return false;
}

// collapse cell
int Cell::collapse(std::vector<int> &freqRules)
{
	// create a "wheel" of possible states
	// number of times a tile appears on the wheel is determiend by frequency rules
	// choose randomly from the wheel

	std::vector<int> wheel;
	int possStateIndex = -1;
	for (int i = 0; i < possStates.size(); i++)
	{
		for (int j = 0; j < freqRules[i]; j++)
		{
			if (possStates[i] > 0)
				wheel.push_back(i);
		}
		if (possStates[i] > 0)
			possStateIndex = i;
	}

	if (wheel.size() > 1)
	{
		int randIndex = rand() % (wheel.size() - 1);
		collapsedState = wheel[randIndex];
	}
	else if (possStateIndex != -1)
	{
		collapsedState = possStateIndex;
	}
	else
	{
		collapsedState = -1;
	}
	return collapsedState;
}

int Cell::getCollapsedState()
{
	return collapsedState;
}

int Cell::getCoords()
{
	return coords;
}

void Cell::updateTileWeights(int tileFreq)
{
	sumTileWeights -= tileFreq;
	logSumTileWeights -= tileFreq * log2(tileFreq);
}

bool Cell::getPossTile(int index)
{
	if (possStates[index] > 0)
		return true;
	return false;
}

int Cell::getPossStateSize()
{
	return possStates.size();
}

float Cell::getEntropy() const
{
	return log2(sumTileWeights) - logSumTileWeights / sumTileWeights;
}