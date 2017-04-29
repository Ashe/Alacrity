#include "TileStart.h"

void TileStart::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);
	setHidden(true);

	safe = true;
}

void TileStart::Render(float dTime, float zAdjust, bool setInsteadOfAdd, bool matSet)
{
	//MATERIAL
	pMat = &tile.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Set(Vector4(1.0f, 0.0f, 0.0f, 0), Vector4(1.0f, 0.0f, 0.0f, 0), Vector4(0, 0, 0, 1));
	pMat->pTextureRV = mFX->mCache.LoadTexture("block.dds", true, gd3dDevice);
	pMat->texture = "block";

	Tile::Render(dTime, zAdjust, setInsteadOfAdd, true);

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