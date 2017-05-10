#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileFloor.h"
#include "TilePickup.h"
#include "TileInvisibleWall.h"
#include "TileInverseInvisibleWall.h"
#include "TileStart.h"
#include "TileEnd.h"

#include "FX.h"

#include "D3D.h"
#include "GeometryBuilder.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class Level
{
public:
	//start up and shut down
	Level(FX::MyFX& fxRef,MeshManager& meshMGR, const Vector3& anch)
		: mFX(&fxRef), mMeshMgr(&meshMGR), anchorPos(anch)
	{
		Load(meshMGR);
	}

	~Level() {
		Release();
	}

	struct TextureInfo
	{

	} textureInfo;

	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);

	// Called everytime a new level is created
	void Initialise(const TextureInfo& textInf, const vector<vector<Tile::TileType>>& layout, const vector<vector<int>>& floorLayout, const vector<vector<int>>& extraInfo, const string& caption, float levelTime, float width = 1, float padding = 1.25, int dim = 10, float safeTime = 5, float fallSpeedSafe = 3, float fallSpeedDead = 10);

	// player interaction
	Vector3 move(const Vector3& pos, const Vector2& dir, bool& success);
	Vector3 getStartingPosition() const;
	Vector3 getEndingPosition() const;
	Vector3 getCurrentLocationOfTile(const Vector3& pos);

	float getTimer() const;

	void Release();

	//handy rendering state
	FX::MyFX* mFX;

	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;

	// Gets whether the level has started/ended
	bool getLevelStarted() const;
	bool getLevelEnded() const;
	bool getLevelSwitch() const;
	bool getWinStatus() const;

	// Get any messages to print to the UI
	string getMessage() const;

private:
	int pickupNo_;
	int collectedNo_;
	int trueCollectedNo_; // This is necessary so that, when checked, the collectedNo isn't in the process of counting
	const Vector3 anchorPos;
	float tileWidth;
	string levelCaption;

	float levelMaxTime;
	Timer timer;

	bool playerBeatLevel;		// When the level transitions, this is used to check whether to restart or proceed

	bool levelFinishedLoading;  // For pausing any updates called in the Init function
	bool levelReadyToPlay;		// When the level has finished transitioning
	bool playerInPlay;			// The player has moved away from the start and is playing
	bool endTheGame;			// The player has touched the end tile of the game or died
	bool readyToTransition;		// The level has 'exploded' and is ready to change level

	bool playerFrozen;			// Whether the player can move

	int cellDim;
	float tilePadding;
	float additionalPadding;
	float maxAdditionalPadding;
	float levelTransitionSpeed;

	float waveEffect;
	bool waveLevel;

	// Arrays of tile pointers
	vector<vector<TileFloor*>> floor;
	vector<vector<Tile*>> level;

	//ensure each mesh is loaded and release exactly once
	MeshManager* mMeshMgr;

	// Load all the meshes required to build a level
	void Load(MeshManager meshMGR);

	// Tile creation
	Tile* createTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch, int info);
	TileFloor* createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch, float safeTime, float fallSpeedSafe, float fallSpeedDead, int protection = 0);

	// Locating functions
	Vector2 getCellFromCoords(const Vector3& pos) const;
	Vector3 getCoordsFromCell(const Vector2& cell, const Vector3& prevPos = Vector3(0, 0, 0)) const;

	//Count number of pickups on current level
	void countPickups(const Tile::TileType& layout);

	// Stat updating
	void checkTile(Tile* tile);
	void checkTileFloor(TileFloor* tile);

	// Controlling the flow of play
	void checkGameState(float dTime);		
};

#endif




