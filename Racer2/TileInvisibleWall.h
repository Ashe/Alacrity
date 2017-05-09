#ifndef TILEINVISIBLEWALLH
#define TILEINVISIBLEWALLH

#include "Tile.h"

class TileInvisibleWall : public Tile {
public:
	TileInvisibleWall(FX::MyFX&(fxRef), const TileType& type, int x, int y, float width, float pad, const Vector3& anch, int gridDim)
		:Tile(fxRef, type, x, y, width, pad, anch, gridDim, true, false) {};

	bool bump() override;

};

#endif