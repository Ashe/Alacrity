#include "TileEnd.h"

void TileEnd::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);
	setHidden(true);

	endReached = false;
}

void TileEnd::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	Tile::Render(dTime, zAdjust, setInsteadOfAdd);
}

void TileEnd::moveOn()
{
	endReached = true;
}

int TileEnd::getInfo() const
{
	// Returns if the game is in play
	return endReached;
}