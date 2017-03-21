#ifndef TILEEND_H
#define TILEEND_H

#include "Tile.h"
class TileEnd : public Tile
{
public:
	TileEnd(const TileType& type, int x, int y, float width, float pad, const Vector3& anch, bool isHidden = false, bool isTraverseable = false)
		:Tile(type, x, y, width, pad, anch, isHidden, isTraverseable) {};

	void Initialise(Mesh& tileMesh) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	// movement functions
	void moveOn() override;

	int getInfo() const override;

private:
	bool endReached;

};
#endif