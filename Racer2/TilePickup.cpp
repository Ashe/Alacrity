#include "TilePickup.h"

void TilePickup::Initialise(Mesh & tileMesh)
{
	Tile::Initialise(tileMesh);

	spinAngle = 0;
	isCollected = false;
}

void TilePickup::Update(float dTime)
{
}

void TilePickup::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	spinAngle += dTime;

	Tile::Render(dTime, zAdjust, setInsteadOfAdd);
	tile.GetRotation() = Vector3(PI / 4, 0, spinAngle);
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

void TilePickup::collect()
{
	isCollected = true;
	setHidden(true);
}
