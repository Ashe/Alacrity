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
	TileFloor(int x, int y, float width, float pad, const Vector3& anch, bool hide = 0)
		:Tile(x, y, width, pad, anch, hide) {}

	void Render(float dTime) override;

private:

};

#endif




