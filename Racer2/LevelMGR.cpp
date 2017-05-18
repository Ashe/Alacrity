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

void LevelMGR::Initialise(int currLevel)
{
	//BuildCube(*mMeshMgr);

	currentLevel = currLevel;
	//currentLevel = 9;
	maxLevels = 10;

	changeLevel(currentLevel);
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
		if (level.getWinStatus())
			currentLevel += 1;

		if (currentLevel > maxLevels || currentLevel < 1)
			currentLevel = 1;

		changeLevel(currentLevel);
	}
}


void LevelMGR::Render(float dTime)
{
	level.Render(dTime);
}

void LevelMGR::changeLevel(int levelNo) {
	nextLevel = getLayout(levelNo);
	level.Initialise(texInfo, nextLevel.tileList, nextLevel.floorList, nextLevel.extraInfoList, nextLevel.levelCaption, nextLevel.levelTime, nextLevel.tileWidth, nextLevel.tilePadding, nextLevel.tileDim, nextLevel.safeTime, nextLevel.fallSpeedSafe, nextLevel.fallSpeedDead);
}

void LevelMGR::forceRandomLevel() {
	level.forceLevelEnd();

	int tempLevel = currentLevel;
	while (tempLevel == currentLevel)
		tempLevel = rand() % maxLevels + 1;

	currentLevel = tempLevel;
}

// Complex one. Pass in your coordinate position and your direction (0/1 x, 0/1 y) and a bool.
// The function will return the location the player should go to at the end of the function.
// The bool will change depending on a whether a move was made.
Vector2 LevelMGR::move(const Vector2& pos, const Vector2& dir, bool& success)
{
	return level.move(pos, dir, success);
}

Vector2 LevelMGR::getStartingPosition() const
{
	return level.getStartingPosition();
}

Vector3 LevelMGR::getCurrentLocationOfTile(Vector2 & pos)
{
	return level.getCurrentLocationOfTile(pos);
}

LevelMGR::levelTemplate LevelMGR::getLayout(int layoutNo) {

	levelTemplate nextLevel;

	switch (layoutNo)
	{
		case -1: // A TEMPLATE
		{
			nextLevel.levelTime = 100;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv X:";

			nextLevel.safeTime = 20;		// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 30;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1.25;		// Width of tles
			nextLevel.tileDim = 10;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.5;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEnder, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2,-2,-2,-2,-2,-2,-2,-2,-2,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0, 0, 0, 0,-2 },
				{ -2,-2,-2,-2,-2,-2,-2,-2,-2,-2 }
			};

			nextLevel.extraInfoList = {
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 1:
		{
			nextLevel.levelTime = 1;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 1: Reach the end";

			nextLevel.safeTime = 3;			// Players have x seconds before falling to deathh
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 3;		// Width of tles
			nextLevel.tileDim = 4;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 3.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty},
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty},
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEnder, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{-2,-2,-2,-2},
				{-1,-1,-1,-2},
				{-2,-2,-1,-2},
				{-2,-2,-1,-2}
			};

			nextLevel.extraInfoList = {
				{  0, 0, 0, 0 },
				{  0, 0, 0, 0 },
				{  0, 0, 0, 0 },
				{  0, 0, 0, 0 }
			};

			break;
		}
		case 2:
		{
			nextLevel.levelTime = 2;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 2: Watch your step";

			nextLevel.safeTime = 3;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 6;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEnder, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2,-2,-2,-2,-2, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2,-2,-2,-2,-2, -2 }
			};

			nextLevel.extraInfoList = {
				{  0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 3:
		{
			nextLevel.levelTime = 3;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 3: Collect the pickup before finishing";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 5;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eStart, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEnder, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2, 0, 0,-2,-2 },
				{ -2, 0, 0, 0,-2 },
				{  0, 0, 0, 0,-2 },
				{ -2,-2, 0, 0,-2 },
				{ -2,-2, 0,-2,-2 }
			};

			nextLevel.extraInfoList = {
				{  0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0 }
			};

			break;
		}
		case 4:
		{
			nextLevel.levelTime = 3;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 4: Some tiles can be stepped on more than once";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to deathh
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 5;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEnder },
				{ Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2, -2, -2, -2, -2 },
				{ -2,  0, -2, -2, -2 },
				{ -1,  2,  0,  1,  0 },
				{ -2,  0, -2,  0, -2 },
				{ -2, -2, -2, -2, -2 }
			};

			nextLevel.extraInfoList = {
				{ 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 5:
		{
			nextLevel.levelTime = 6.1;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 5: A little more complex";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 9;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eStart, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEnder, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },

			};

			nextLevel.floorList = {
				{ -2, -2, -2, -2, -2, -2, -2, -2, -2 },
				{ -2, -2, -2, 0, 2, 0, -2, -2, -2 },
				{ -2, -2, -2, -2, 1, -2, -2, -2, -2 },
				{ -2, -2, -2, 0, 1, 0, -2, -2, -2 },
				{ -2, -2, 0, 1, 0, 1, 0, -2, -2 },
				{ -2, -2, -2, 0, 1, 0, -2, -2, -2 },
				{ -2, -2, -2, -2, 1, -2, -2, -2, -2 },
				{ -2, -2, -2, 0, 2, 0, -2, -2, -2 },
				{ -2, -2, -2, -2, -2, -2, -2, -2, -2 }
			};

			nextLevel.extraInfoList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 6:
		{
			nextLevel.levelTime = 7;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 6: Plan your moves";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1;		// Width of tles
			nextLevel.tileDim = 8;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic },
				{ Tile::eBasic, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::ePikup, Tile::ePikup, Tile::eEnder },
				{ Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eStart, Tile::ePikup, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::ePikup, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::ePikup, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
			};

			nextLevel.floorList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 0, 0 }
			};

			nextLevel.extraInfoList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 7:
		{
			nextLevel.levelTime = 7;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 7: Tread Carefully";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 30;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1.5;		// Width of tles
			nextLevel.tileDim = 7;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.75;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::ePikup, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEnder, Tile::ePikup, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eStart },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::ePikup, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2,-2,-2,-2,-2,-2,-2 },
				{ -2, 0, 0, 0, 0, 0,-2 },
				{ -2, 0, 2, 1, 2, 0,-2 },
				{  0, 0, 1, 0, 1, 1, 0 },
				{ -2, 0, 1, 1, 1, 1,-2 },
				{ -2, 0, 0, 0, 0, 0,-2 },
				{ -2,-2,-2,-2,-2,-2,-2 }
			};

			nextLevel.extraInfoList = {
				{  0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 2, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0 },
				{  0, 4, 0, 0, 0, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0 },
				{  0, 0, 3, 0, 1, 0, 0 },
				{  0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 8:
		{
			nextLevel.levelTime = 8;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 8: ";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 30;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1.75;		// Width of tles
			nextLevel.tileDim = 7;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.5;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEnder, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eStart },
				{ Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::ePikup, Tile::eEmpty, Tile::eBasic, Tile::ePikup, Tile::eBasic, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2,-2,-2,-2,-2,-2,-2 },
				{ -2, 0, 1, 1, 0, 0,-2 },
				{ -2, 0, 0, 0, 0, 0,-2 },
				{  0, 0, 1, 0, 0, 1, 0 },
				{ -2, 0, 2, 1, 2, 0,-2 },
				{ -2, 0, 1, 0, 0, 0,-2 },
				{ -2,-2,-2,-2,-2,-2,-2 }
			};

			nextLevel.extraInfoList = {
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		case 9:
		{
			nextLevel.levelTime = 11;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 9: ";

			nextLevel.safeTime = 2;			// Players have x seconds before falling to death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1;		// Width of tles
			nextLevel.tileDim = 8;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup },
				{ Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eEmpty },
				{ Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::ePikup, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::ePikup, Tile::eBasic, Tile::eEmpty },
				{ Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eEmpty },
				{ Tile::eEnder, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::ePikup },
			};

			nextLevel.floorList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 1, 0, 0, 0, 0, 0 },
				{ 0, 0, 1, 0, 0, 1, 0, 0 },
				{ 0, 0, 1, 0, 0, 1, 0, 0 },
				{ 0, 0, 1, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 }
			};

			nextLevel.extraInfoList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 }
			};

			break;
		}
		default:
			return getLayout(1);
	}
	return nextLevel;
}

int LevelMGR::getPickupNo() const
{
	return level.getPickupNo();
}

int LevelMGR::getCollectedNo() const
{
	return level.getCollectedNo();
}

float LevelMGR::getTimer() const
{
	return level.getTimer();
}

float LevelMGR::getLevelMaxTime() const {
	return nextLevel.levelTime;
}


string LevelMGR::getMessage() const
{
	return level.getMessage();
}

bool LevelMGR::getLevelStarted() const
{
	return level.getLevelStarted();
}

void LevelMGR::reset(){
	currentLevel = 1;
	changeLevel(currentLevel);
}