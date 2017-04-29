#ifndef TILEEND_H
#define TILEEND_H

#include "Tile.h"
class TileEnd : public Tile
{
public:
	TileEnd(FX::MyFX& fxRef, const TileType& type, int x, int y, float width, float pad, const Vector3& anch, int gridDim, bool isHidden = false, bool isTraverseable = false)
		:Tile(fxRef, type, x, y, width, pad, anch, gridDim, isHidden, isTraverseable) {};

	void Initialise(Mesh& tileMesh) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false, bool matSet = false) override;

	// movement functions
	void moveOn() override;

	int getInfo() const override;

private:
	bool endReached;

};
#endif
