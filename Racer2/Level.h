#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileFloor.h"
#include "TilePickup.h"
#include "TileStart.h"
#include "TileEnd.h"

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
	Level(const MeshManager& meshMGR, const Vector3& anch, const float width)
		: mMeshMgr(meshMGR), anchorPos(anch), tileWidth(width), pickupNo_(0)
	{
		Load(meshMGR);
	}

	~Level() {
		Release();
	}

	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);

	// Called everytime a new level is created
	void Initialise(const vector<vector<Tile::TileType>>& layout, float padding = 1.25, int dim = 10);

	// player interaction
	Vector3 move(const Vector3& pos, const Vector2& dir, bool& success);
	Vector3 getStartingPosition() const;
	Vector3 getEndingPosition() const;
	Vector3 getCurrentLocationOfTile(const Vector3& pos);

	void Release();

	//handy rendering state
	FX::MyFX mFX;


	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;

	// Gets whether the level has started/ended
	bool getLevelStarted() const;
	bool getLevelEnded() const;
	bool getLevelSwitch() const;

private:
	int pickupNo_;
	int collectedNo_;
	const Vector3 anchorPos;
	const float tileWidth;

	bool levelReadyToPlay;
	bool playerInPlay;
	bool playerReachedGoal;
	bool readyToTransition;

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
	MeshManager mMeshMgr;
	//vector<Mesh> meshArray;


	// Load all the meshes required to build a level
	void Load(MeshManager meshMGR);

	// Tile creation
	Tile* createTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch);
	TileFloor* createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch);

	// Locating functions
	Vector2 getCellFromCoords(const Vector3& pos) const;
	Vector3 getCoordsFromCell(const Vector2& cell, const Vector3& prevPos = Vector3(0, 0, 0)) const;

	//Count number of pickups on current level
	void countPickups(const Tile::TileType& layout);

	// Stat updating
	void checkLevel();
	void checkTile(Tile* tile);

	// Controlling the flow of play
	void checkGameState(float dTime);	
	
};

#endif




