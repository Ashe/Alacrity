#include "Tile.h"

void Tile::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Tile::Initialise(Mesh& tileMesh)
{
	tile.Initialise(tileMesh);

	// Remove any displacement to reset the location
	adjustVector = { 0, 0, 0 };

	mFX.Init(gd3dDevice);
}

void Tile::Release()
{
	// DELETE DYNAMICALLY ALLOCATED MEMORY (somehow)
}

void Tile::Update(float dTime)
{

}


void Tile::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	if (!hidden)
	{
		mFX.Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = anchor.x + cellX * (cellWidth + padding);
		tile.GetPosition().y = anchor.y + cellY * (cellWidth + padding);

		if (!setInsteadOfAdd)
			tile.GetPosition().z = anchor.z + cellWidth + padding + zAdjust;
		else
			tile.GetPosition().z = zAdjust + padding;

		tile.GetPosition() += adjustVector;
	}
}

///////////////////////////////////////////////////////
// Tile Functions
///////////////////////////////////////////////////////

void Tile::killTile() {
	
	// This function will call if a specific tile should be killed off
	// - unique animations and mechanics to be inserted here
	switch (tileType) {
		default:
			break;
	}
}

///////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////

Vector3 Tile::getPosition() {
	return tile.GetPosition();
}

int Tile::getCellX() {
	return cellX;
}

int Tile::getCellY() {
	return cellY;
}

float Tile::getPadding() {
	return padding;
}
float Tile::getCellWidth() {
	return cellWidth;
}

bool Tile::bump()
{
	return getTraverseable();
}

void Tile::moveOn()
{
}

void Tile::moveOff()
{
}

Vector3 Tile::getAnchor() {
	return anchor;
}

Vector3 Tile::getDisplacement() {
	return adjustVector;
}

bool Tile::getHidden() const{
	return hidden;
}

void Tile::setHidden(bool b)
{
	hidden = b;
}

bool Tile::getTraverseable() const
{
	return traverseable;
}

void Tile::setTraverseable(bool b)
{
	traverseable = b;
}

Tile::TileType Tile::getTileType() {
	return tileType;
}

///////////////////////////////////////////////////////
// Setters
///////////////////////////////////////////////////////

void Tile::addDisplacement(const Vector3& disp) {
	adjustVector += disp;
}

void Tile::resetDisplacement() {
	adjustVector = { 0, 0, 0 };
}