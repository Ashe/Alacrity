#include "TilePickup.h"

void TilePickup::Initialise(Mesh & tileMesh)
{
	Tile::Initialise(tileMesh);

	spinAngle = 0;
	isCollected = false;
}

void TilePickup::Render(float dTime, float zAdjust, bool setInsteadOfAdd, bool matSet)
{
	spinAngle += dTime;

	Tile::Render(dTime, zAdjust, setInsteadOfAdd);
	tile.GetRotation() = Vector3(PI / 4, PI / 4, spinAngle);
	tile.GetScale() = Vector3(0.5, 0.5, 0.5);
}

bool TilePickup::bump()
{
	return Tile::bump();
}

void TilePickup::moveOn()
{
	if (!isCollected && !getHidden())
		collect();
}

void TilePickup::moveOff()
{
}

int TilePickup::getInfo(int amountCollected)
{
	setHidden((amountCollected < reqCollectedToShow) || isCollected);

	// Use the info received to update self
	return getCollected();
}

void TilePickup::collect()
{
	isCollected = true;
	setHidden(true);
}

bool TilePickup::getCollected() const
{
	return isCollected;
}

