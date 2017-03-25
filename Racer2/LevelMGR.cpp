#include <thread>
#include <iomanip>

#include "WindowUtils.h" 
#include "D3D.h"
#include "LevelMGR.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void LevelMGR::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void LevelMGR::Initialise()
{
	BuildCube(*mMeshMgr);

	changeLevel(0);
}

void LevelMGR::Release()
{
	level.Release();
}

void LevelMGR::Update(float dTime)
{
	level.Update(dTime);
	if (level.getLevelSwitch())
	{
		changeLevel(0);
	}
}


void LevelMGR::Render(float dTime)
{
	level.Render(dTime);
}

void LevelMGR::changeLevel(int levelNo) {
	levelTemplate nextLevel = getLayout(levelNo);
	level.Initialise(texInfo, nextLevel.tileList, nextLevel.tilePadding, nextLevel.tileDim, nextLevel.safeTime, nextLevel.fallSpeedSafe, nextLevel.fallSpeedDead);
}

// Complex one. Pass in your coordinate position and your direction (0/1 x, 0/1 y) and a bool.
// The function will return the location the player should go to at the end of the function.
// The bool will change depending on a whether a move was made.
Vector3 LevelMGR::move(const Vector3& pos, const Vector2& dir, bool& success)
{
	return level.move(pos, dir, success);
}

Vector3 LevelMGR::getStartingPosition() const
{
	return level.getStartingPosition();
}

Vector3 LevelMGR::getCurrentLocationOfTile(const Vector3 & pos)
{
	return level.getCurrentLocationOfTile(pos);
}

LevelMGR::levelTemplate LevelMGR::getLayout(int layoutNo) {

	levelTemplate nextLevel;

	switch (layoutNo)
	{
		case 0:
		{
			nextLevel.tileDim = 10;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.25;	// Padding between blocks
			nextLevel.safeTime = 3;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileList = {

				{Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::ePickup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eStart, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::ePickup, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEnd, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic}

			};
			break;
		}
	}

	return nextLevel;
}

int LevelMGR::getLayoutDim(int levelNo) {
	return 0;
}

int LevelMGR::getPickupNo() const
{
	return level.getPickupNo();
}

int LevelMGR::getCollectedNo() const
{
	return level.getCollectedNo();
}

bool LevelMGR::getLevelStarted() const
{
	return level.getLevelStarted();
}
