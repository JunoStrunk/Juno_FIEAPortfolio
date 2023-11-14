#include <iostream>
#pragma once

/*
Removal card fills up the removal stack.
These cards hold coords and removed tile index
These cards have had tiles eliminated from them and therefore their neighbors must be checked so that contradictions can be eliminated.

MAJOR INFORMATION LEARNED:
Having a removal stack greatly slows down collapsing because many possibilities must be iterated through each step
BUT
Removing possible contradictions throughout each collapse overall speeds up the process
as we are less likely restart by running into a contradiction.
Correctly removing enablers with the removal stack allowed certain sample images to generate 40x40 tile maps easily (DMMiniWorld)
vs before when 40x40 would not have even been possible.

It feels like a monkey on a typewriter vs teaching a monkey to type
*/
class RemovalCard
{
private:
	int tileIndex;
	int coords;

public:
	RemovalCard(int tileIndex, int coords)
	{
		this->tileIndex = tileIndex;
		this->coords = coords;
	};

	RemovalCard &operator=(const RemovalCard &rhs)
	{
		this->tileIndex = rhs.tileIndex;
		this->coords = rhs.coords;
		return *this;
	}

	int getRemovedTile()
	{
		return tileIndex;
	}

	int getRemovedCoords()
	{
		return coords;
	}
};