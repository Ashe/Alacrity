#include "TileStart.h"

void TileStart::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);
	setHidden(true);

	safe = true;
}

void TileStart::Update(float dTime)
{
}

void TileStart::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	Tile::Render(dTime, zAdjust, setInsteadOfAdd);
}

bool TileStart::bump()
{
	return true;
}

void TileStart::moveOn()
{
}

void TileStart::moveOff()
{
	safe = false;
}

int TileStart::getInfo() const
{
	// Returns if the game is in play
	return !safe;
}