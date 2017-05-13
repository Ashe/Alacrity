#ifndef LEVELMGR_H
#define LEVELMGR_H

#include "Level.h"
#include "Tile.h"
#include "FX.h"
#include "Singleton.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class LevelMGR : public Singleton<LevelMGR>
{
public:
	//start up and shut down
	LevelMGR(FX::MyFX* fxRef, MeshManager& meshMGR, const Vector3& anch)
		: mFX(fxRef), mMeshMgr(&meshMGR), level(fxRef, meshMGR, anch)
	{}
	~LevelMGR() {
		Release();
	}
	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	void Initialise();
	void Release();

	void changeLevel(int levelNo);

	//player interaction
	Vector2 move(const Vector2& pos, const Vector2& dir, bool& success);
	Vector2 getStartingPosition() const;
	Vector3 getCurrentLocationOfTile(Vector2& pos);

	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;
	float getTimer() const;
	string getMessage() const;

	// Get whether the player has started
	bool getLevelStarted() const;

private:

	struct levelTemplate
	{
		float levelTime;		// The time the player has to complete the level
		string levelCaption;	// The hint of the level displayed on the UI

		float safeTime;			// The time the player can spend on a tile before they die
		float fallSpeedSafe;	// The speed of the falling tile before the player jumps off
		float fallSpeedDead;	// The speed of the tile after the player has left

		float tileWidth;		// The size of the tiles
		int tileDim;			// The size of the grid
		float tilePadding;		// The size of the padding between the tiles

		vector<vector<Tile::TileType>> tileList;
		vector<vector<int>> floorList;
		vector<vector<int>> extraInfoList;
	};

	Level::TextureInfo texInfo;

	int currentLevel;

	//ensure each mesh is loaded and release exactly once
	MeshManager* mMeshMgr;

	//handy rendering state
	FX::MyFX* mFX;

	levelTemplate getLayout(int LevelNo);

	Level level;

	int pickupNo_;
};
SINGLETON_GET(LevelMGR);
#endif