#ifndef TILEPICKUP_H
#define TILEPICKUP_H

#include "Tile.h"
class TilePickup : public Tile
{
public:
	TilePickup(const TileType& type, int x, int y, float width, float pad, const Vector3& anch, bool isHidden = false, bool isTraverseable = false)
		:Tile(type, x, y, width, pad, anch, isHidden, isTraverseable) {};

	void Initialise(Mesh& tileMesh) override;
	void Update(float dTime) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	// movement functions
	bool bump() override;
	void moveOn() override;
	void moveOff() override;

	bool getCollected() const override;
	

private:

	float spinAngle;
	bool isCollected;

	void collect();
};
#endif
