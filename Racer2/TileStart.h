#ifndef TILESTART_H
#define TILESTART_H

#include "Tile.h"
class TileStart : public Tile
{
public:
	TileStart(FX::MyFX& fxRef, const TileType& type, int x, int y, float width, float pad, const Vector3& anch, int gridDim, bool isHidden = false, bool isTraverseable = false)
		:Tile(fxRef, type, x, y, width, pad, anch, gridDim, isHidden, isTraverseable) {};

	void Initialise(Mesh& tileMesh) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false, bool matSetAlready = false) override;

	// movement functions
	void moveOff() override;

	int getInfo(int = 0)  override;

private:
	bool safe;

};
#endif
