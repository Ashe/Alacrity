#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileFloor.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class Level
{
public:

	enum tileType
	{
		eEmpty, eFloor, eBasic
	};

	//start up and shut down
	Level(const MeshManager& meshMGR, const Vector3& anch, const float width)
		: mMeshMgr(meshMGR), anchorPos(anch), tileWidth(width)
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
	void Initialise(const vector<vector<tileType>>& layout, float padding = 1.25, int dim = 10);

	void Release();

	//handy rendering state
	FX::MyFX mFX;

private:
	const Vector3 anchorPos;
	const float tileWidth;

	int cellDim;
	float tilePadding;

	// Arrays of tiles
	vector<vector<Tile>> background;
	vector<vector<Tile>> level;

	//ensure each mesh is loaded and release exactly once
	MeshManager mMeshMgr;
	//vector<Mesh> meshArray;

	// Create a tile from the enum
	Tile getTile(const tileType& type, int cellX, int cellY);

	// Load all the meshes required to build a level
	void Load(MeshManager meshMGR);


};

#endif




