#include "TileEnd.h"

void TileEnd::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);
	setHidden(true);

	endReached = false;
	tileKilled = false;
}

void TileEnd::Render(float dTime, float zAdjust, bool setInsteadOfAdd, bool matSet)
{
	Tile::Render(dTime, zAdjust, setInsteadOfAdd);
}

void TileEnd::moveOn()
{
	endReached = true;
}

void TileEnd::moveOff()
{
	endReached = false;
	tileKilled = true;
}

int TileEnd::getInfo(int) 
{
	// Returns if the game is in play
	return endReached && !tileKilled;
}