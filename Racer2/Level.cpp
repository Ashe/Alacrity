#include <thread>
#include <iomanip>

#include "WindowUtils.h" 
#include "D3D.h"
#include "Level.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Level::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Level::Initialise(const vector<vector<Tile::TileType>>& layout,float padding, int dim)
{
	mFX.Init(gd3dDevice);

	// Set level specific params
	cellDim = dim;
	tilePadding = padding;

	floor.empty();
	level.empty();

	// DELETE THIS LATER
	Mesh& cubeMesh = BuildCube(mMeshMgr);

	// Initialise both foreground and background
	for (int i = 0; i < dim; i++) {
		vector<Tile*> tempLevel;
		vector<TileFloor*> tempBack;

		for (int j = 0; j < dim; j++) {
			// Create and intialise a floor tile and place it on the background grid
			tempBack.push_back(createFloorTile(layout[i][j], j, i, tileWidth, tilePadding, anchorPos));
			tempBack[j]->Initialise(cubeMesh);// meshArray[1]);

			// Create and initialise a tile given in the layout, created in the levelMGR
			tempLevel.push_back(createTile(layout[i][j],j, i, tileWidth,tilePadding,anchorPos));
			tempLevel[j]->Initialise(cubeMesh);// meshArray[layout[i][j]]);
		}

		// Add the row of tiles to the level's storage and repeat
		floor.push_back(tempBack);
		level.push_back(tempLevel);
	}

	// Fancy effects
	waveEffect = 0;
	waveLevel = true;
}

void Level::Release()
{

}

void Level::Update(float dTime)
{
	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			floor[i][j]->Update(dTime);  
			level[i][j]->Update(dTime);
		}
}


void Level::Render(float dTime)
{
	waveEffect += dTime * 1.5;

	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			floor[i][j]->Render(dTime, (sinf(waveEffect + (i + j) / (PI / 2))) / 4);  
			level[i][j]->Render(dTime, floor[i][j]->getPosition().z, true);
		}
}

void Level::Load(MeshManager meshMGR)
{
	// TODO: Load meshes from an array to coincide with ENUM

	//Mesh& cubeMesh = BuildCube(mMeshMgr);

	//// The following list is in sync with the enum - the order these are inserted into the mesh array are important
	//meshArray.push_back(cubeMesh);
	//meshArray.push_back(cubeMesh);
	//meshArray.push_back(cubeMesh);
}

Tile* Level::createTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch)
{
	switch (type) {
	case Tile::TileType::eBasic:
		return new Tile(type, x, y, width, pad, anch);
		break;

	case Tile::TileType::eEmpty:
		return new Tile(type, x, y, width, pad, anch, true);
		break;
	}
}

TileFloor* Level::createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch)
{
	switch (type) {
	default:
		return new TileFloor(type, x, y, width, pad, anch);
		break;
	}
}
