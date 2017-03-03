#include "TileFloor.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void TileFloor::Render(float dTime)
{

	if (!getHidden())
	{
		mFX.Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = getAnchor().x + getCellX() * (getCellWidth() + getPadding());
		tile.GetPosition().y = getAnchor().y + getCellY() * (getCellWidth() + getPadding());
		tile.GetPosition().z = getAnchor().z;

		tile.GetScale().x = 20;
	}
}