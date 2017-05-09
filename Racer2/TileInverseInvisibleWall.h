#ifndef TILEINVERSEINVISIBLEWALLH
#define TILEINVERSEINVISIBLEWALLH

#include "Tile.h"

class TileInverseInvisibleWall : public Tile {
public:
	TileInverseInvisibleWall(FX::MyFX&(fxRef), const TileType& type, int x, int y, float width, float pad, const Vector3& anch, int gridDim)
		:Tile(fxRef, type, x, y, width, pad, anch, gridDim, false, true) {};

	bool bump() override;

};

#endif