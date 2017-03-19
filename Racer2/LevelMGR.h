#ifndef LEVELMGR_H
#define LEVELMGR_H

#include "Level.h"
#include "Tile.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class LevelMGR
{
public:
	//start up and shut down
	LevelMGR(const MeshManager& meshMGR, const Vector3& anch, float width)
		: mMeshMgr(meshMGR), level(meshMGR, anch, width)
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
	Vector3 move(const Vector3& pos, const Vector2& dir, bool& success);
	float getZOfTile(const Vector3& pos);

	//handy rendering state
	FX::MyFX mFX;

	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;

private:

	struct levelTemplate
	{
		int tileDim;
		float tilePadding;
		vector<vector<Tile::TileType>> tileList;
	};

	int currentLevel;

	//ensure each mesh is loaded and release exactly once
	MeshManager mMeshMgr;

	int getLayoutDim(int levelNo);
	levelTemplate getLayout(int LevelNo);

	Level level;

	int pickupNo_;
};

#endif