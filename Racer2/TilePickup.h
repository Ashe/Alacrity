#ifndef TILEPICKUP_H
#define TILEPICKUP_H

#include "Tile.h"
class TilePickup : public Tile
{
public:
	TilePickup(FX::MyFX& fxRef, const TileType& type, int x, int y, float width, float pad, const Vector3& anch, int gridDim, bool isHidden = false, bool isTraverseable = false, int collectedReq = 0)
		:Tile(fxRef, type, x, y, width, pad, anch, gridDim, isHidden, isTraverseable), reqCollectedToShow(collectedReq) {};

	void Initialise(Mesh& tileMesh) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false, bool matSet = false) override;

	// movement functions
	bool bump() override;
	void moveOn() override;
	void moveOff() override;

	bool getCollected() const;
	int getInfo(int = 0)  override;

private:

	float spinAngle;
	bool isCollected;
	int reqCollectedToShow;

	void collect();
};
#endif
