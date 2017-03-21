#ifndef TILEFLOOR_H
#define TILEFLOOR_H

#include "Tile.h"
#include "D3D.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class TileFloor : public Tile
{
public:
	//start up and shut down
	TileFloor(const TileType& type, int x, int y, float width, float pad, const Vector3& anch, bool isHidden = false, bool isTraverseable = false)
		:Tile(type, x, y, width, pad, anch, isHidden, isTraverseable) {}

	void Initialise(Mesh& tileMesh) override;
	void Update(float dTime, float newPadding = -1) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	void killTile() override;

private:
	enum DestroyStatus
	{
		stable, unstable, degrading, destroyed
	} destroyStatus;

};

#endif




