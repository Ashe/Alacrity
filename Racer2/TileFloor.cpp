#include "TileFloor.h"

void TileFloor::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);

	destroyStatus = stable;
}

void TileFloor::Update(float dTime, float newPadding) {

	Tile::Update(dTime, newPadding);


	switch (destroyStatus) {
		case stable:
			// A happy floor tile
			break;
		case unstable:
			// When the player is standing on the tile, soon to be destroyed
			break;
		case degrading:
			addDisplacement(Vector3(0, 0, -0.005f));
			// When the floor is actually being destroyed
			break;
		case destroyed:
			// When the floor has disappeared
			break;
	}
}

void TileFloor::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	if (!getHidden())
	{
		mFX.Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = getAnchor().x + (getCellX() - getDim() / 2) * (getCellWidth() + getPadding());
		tile.GetPosition().y = getAnchor().y + (getCellY() - getDim() / 2) * (getCellWidth() + getPadding());

		tile.GetScale().z = 0.2;

		if (!setInsteadOfAdd)
			tile.GetPosition().z = getAnchor().z + getCellWidth() - (getCellWidth() * tile.GetScale().z) + zAdjust + getPadding();
		else
			tile.GetPosition().z = zAdjust;

		tile.GetPosition() += getDisplacement();
	}
}

///////////////////////////////////////////////////////
// TileFloor Functions
///////////////////////////////////////////////////////

void TileFloor::killTile() {

	// This function completely overrides the normal killTile
	// - as it's specific to the floor - we may not want normal tiles to fall

	// Set the status to the melting/falling animation
	destroyStatus = degrading;

}