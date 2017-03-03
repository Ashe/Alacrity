#include "TileFloor.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void TileFloor::Initialise(Mesh& tileMesh)
{
	tile.Initialise(tileMesh);

	mFX.Init(gd3dDevice);
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
	}
}