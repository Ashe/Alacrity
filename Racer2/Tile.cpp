#include "Tile.h"

void Tile::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Tile::Initialise(Mesh& tileMesh)
{
	tile.Initialise(tileMesh);

	pMat = &tile.GetMesh().GetSubMesh(0).material;
	
	// Remove any displacement to reset the location
	adjustVector = { 0, 0, 0 };
}

void Tile::Release()
{
	// Release the used data (somehow)
}

void Tile::Update(float dTime, float newPadding)
{
	if (newPadding > 0)
		padding = newPadding;
}


void Tile::Render(float dTime, float zAdjust, bool setInsteadOfAdd, bool matSetAlready)
{
	
	if (!matSetAlready) {
		//MATERIAL
		pMat->gfxData.Set(Vector4(1.0f, 1.0f, 1.0f, 0), Vector4(1.0f, 1.0f, 1.0f, 0), Vector4(0, 0, 0, 1));
		pMat->pTextureRV = mFX->mCache.LoadTexture("block.dds", true, gd3dDevice);
		pMat->texture = "block";
	}

	if (!hidden)
	{
		mFX->Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = anchor.x + (cellX - gridDimensions / 2) * (cellWidth + padding);
		tile.GetPosition().y = anchor.y + (cellY - gridDimensions / 2) * (cellWidth + padding);

		if (!setInsteadOfAdd)
			tile.GetPosition().z = anchor.z + cellWidth + padding + zAdjust;
		else
			tile.GetPosition().z = zAdjust + 1.1 * padding;

		tile.GetPosition() += adjustVector;

		tile.GetScale() = Vector3(cellWidth, cellWidth, cellWidth);
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

int Tile::getDim()
{
	return gridDimensions;
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

int Tile::getInfo() const
{
	return 0;
}