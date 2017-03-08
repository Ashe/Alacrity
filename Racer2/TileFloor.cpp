#include "TileFloor.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void TileFloor::Initialise(Mesh& tileMesh)
{
	tile.Initialise(tileMesh);

	mFX.Init(gd3dDevice);

	destroyStatus = stable;

	// DEBUG
	if (getCellX() == 0 && getCellY() == 0)
		killTile();

}

void TileFloor::Update(float dTime) {
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

void TileFloor::Render(float dTime)
{

	if (!getHidden())
	{
		mFX.Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = getAnchor().x + getCellX() * (getCellWidth() + getPadding());
		tile.GetPosition().y = getAnchor().y + getCellY() * (getCellWidth() + getPadding());

		tile.GetScale().z = 0.2;
		tile.GetPosition().z = getAnchor().z + getCellWidth() - (getCellWidth() * tile.GetScale().z);

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