#include "TileFloor.h"

void TileFloor::Initialise(Mesh& tileMesh)
{
	Tile::Initialise(tileMesh);

	playerIsOn = false;
	floorIsDead = false;
	destroyStatus = stable;

	switch (protection) {
	case -1:
		// Invincible tile
		break;
	case -2:
		// Destroyed tile
		protection = 0;
		floorIsDead = true;
		removeTile();
		break;
	}

}

void TileFloor::Update(float dTime, float newPadding) {

	Tile::Update(dTime, newPadding);

	destroyTimer.updateTimer(dTime);

	switch (destroyStatus) {
		case stable:
			// A happy floor tile
			break;
		case unstable:
			// When the player is standing on the tile, soon to be destroyed if they don't jump off quick
			addDisplacement(Vector3(0, 0, -unstableSpeed * dTime));
			if (destroyTimer.getTimer() <= 0)
				killTile();
			break;
		case degrading:
			// When the floor is actually being destroyed, make it sink
			addDisplacement(Vector3(0, 0, -pingSpeed * dTime));

			if (destroyTimer.getTimer() <= 0)
				destroyStatus = destroyed;
			break;
		case destroyed:
			// When the floor has disappeared, flick a few bools and make it act like normal
			// If Destroyed has been triggered twice, the player caused it and will flag a death
			addDisplacement(Vector3(0, 0, -pingSpeed * dTime));
			if (!floorIsDead) {
				removeTile();
			}
			break;
	}
}

void TileFloor::Render(float dTime, float zAdjust, bool setInsteadOfAdd, bool matSet)
{
	if (!matSet) {
		switch (getTileType()) {
		case Tile::eEnder:
			pMat->pTextureRV = mFX->mCache.LoadTexture("endblock.dds", true, gd3dDevice);
			break;
		default:
			switch (protection) {
			case 1:
				pMat->pTextureRV = mFX->mCache.LoadTexture("dblock.dds", true, gd3dDevice);
				break;
			case 2:
				pMat->pTextureRV = mFX->mCache.LoadTexture("tblock.dds", true, gd3dDevice);
				break;
			default:
				pMat->pTextureRV = mFX->mCache.LoadTexture("block.dds", true, gd3dDevice);
			}
		}
	}
	
	if (!getHidden())
		mFX->Render(tile, gd3dImmediateContext);

	tile.GetPosition().x = getAnchor().x + (getCellX() - getDim() / 2) * (getCellWidth() + getPadding());
	tile.GetPosition().y = getAnchor().y + (getCellY() - getDim() / 2) * (getCellWidth() + getPadding());

	tile.GetScale() = Vector3(getCellWidth(), getCellWidth(), 0.2);

	if (!setInsteadOfAdd)
		tile.GetPosition().z = getAnchor().z + getCellWidth() - (getCellWidth() * tile.GetScale().z) + zAdjust + getPadding();
	else
		tile.GetPosition().z = zAdjust;

	tile.GetPosition() += getDisplacement();
}

Vector3 TileFloor::getPosition()
{
	Vector3 temp = Tile::getPosition();

	if (floorIsDead && protection == -2)
		temp.z = getDisplacement().z + getPadding() + getCellWidth() - (getCellWidth() * tile.GetScale().z);

	return temp;
}

void TileFloor::moveOn()
{
	stepOnTile();
}

void TileFloor::moveOff()
{
	stepOffTile();
}

int TileFloor::getInfo(int) 
{
	if (!playerIsOn || destroyStatus == unstable)
		return 0;		
	else // playerIsOn BUT it's not unstable (therefore DEGRADING OR DESTROYED)
	{
		if (destroyStatus == degrading)
			return 1;
		else 
			return 2;
	}
}

///////////////////////////////////////////////////////
// TileFloor Functions
///////////////////////////////////////////////////////

void TileFloor::stepOnTile()
{
	playerIsOn = true;

	if (!floorIsDead) {
		destroyStatus = unstable;

		destroyTimer.setTimer(unstableTime);
		destroyTimer.startTimer();
	}
	else {
		removeTile();
		killTile();
	}
}

void TileFloor::stepOffTile()
{
	destroyTimer.resetTimer();

	playerIsOn = false;
	if (protection != 0)
	{
		if (protection > 0)
			protection -= 1;
		resetDisplacement();
		destroyStatus = stable;
	}

	else if (protection == 0)
		killTile();
}

void TileFloor::killTile() {

	// Restart the timer and prepare to countdown
	destroyTimer.resetTimer();
	// Set the status to the melting/falling animation (which is timed)
	destroyStatus = degrading;
	floorIsDead = true;

	destroyTimer.setTimer(1); // 1 Second Until Death
	destroyTimer.startTimer();

}

void TileFloor::removeTile()
{
	setHidden(true);
	resetDisplacement();
	destroyStatus = stable;
}
