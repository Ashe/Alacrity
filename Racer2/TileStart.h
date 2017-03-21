#ifndef TILESTART_H
#define TILESTART_H

#include "Tile.h"
class TileStart : public Tile
{
public:
	TileStart(const TileType& type, int x, int y, float width, float pad, const Vector3& anch, bool isHidden = false, bool isTraverseable = false)
		:Tile(type, x, y, width, pad, anch, isHidden, isTraverseable) {};

	void Initialise(Mesh& tileMesh) override;
	void Update(float dTime) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	// movement functions
	bool bump() override;
	void moveOn() override;
	void moveOff() override;

	int getInfo() const override;

private:
	bool safe;

};
#endif
