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

void Level::Initialise(const vector<vector<tileType>>& layout,float padding, int dim)
{
	mFX.Init(gd3dDevice);

	// Set level specific params
	cellDim = dim;
	tilePadding = padding;

	background.empty();
	level.empty();

	// DELETE THIS LATER
	Mesh& cubeMesh = BuildCube(mMeshMgr);

	// Initialise both foreground and background
	for (int i = 0; i < dim; i++) {
		vector<Tile> tempLevel;
		vector<Tile> tempBack;

		for (int j = 0; j < dim; j++) {
			// Create and intialise a floor tile and place it on the background grid
			tempBack.push_back(getTile(eFloor, j, i));
			tempBack[j].Initialise(cubeMesh);// meshArray[1]);

			// Create and initialise a tile given in the layout, created in the levelMGR
			tempLevel.push_back(getTile(layout[i][j], j, i));
			tempLevel[j].Initialise(cubeMesh);// meshArray[layout[i][j]]);
		}

		// Add the row of tiles to the level's storage and repeat
		background.push_back(tempBack);
		level.push_back(tempLevel);
	}
}

void Level::Release()
{

}

void Level::Update(float dTime)
{
	
}


void Level::Render(float dTime)
{
	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			background[i][j].Render(dTime);  
			level[i][j].Render(dTime);
		}
}

Tile Level::getTile(const tileType& type, int cellX, int cellY) {

	// TODO. This function will take the type, find the specialised tile class, create and return it

	switch (type)
	{
		case eEmpty :
			return Tile(cellX, cellY, tileWidth, tilePadding, anchorPos, true);
			break;
		case eFloor :
			return TileFloor(cellX, cellY, tileWidth, tilePadding, anchorPos);
			break;
		case eBasic :
			return Tile(cellX, cellY, tileWidth, tilePadding, anchorPos);
			break;
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