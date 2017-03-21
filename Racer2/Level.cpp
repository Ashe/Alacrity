#include "Level.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Level::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Level::Initialise(const vector<vector<Tile::TileType>>& layout,float padding, int dim)
{
	pickupNo_ = 0;

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

			countPickups(layout[i][j]);
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
	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			floor[i][j]->Release();
			level[i][j]->Release();
		}
}

void Level::Update(float dTime)
{
	// Re-counting whether something is collected
	collectedNo_ = 0;

	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			floor[i][j]->Update(dTime);  
			level[i][j]->Update(dTime);

			// Count how many pickups have been collected
			if (level[i][j]->getTileType() == Tile::ePickup && level[i][j]->getInfo()){
				collectedNo_++;
			}
		}
}


void Level::Render(float dTime)
{
	if (waveLevel)
		waveEffect += dTime * 1.5;

	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {

			if (waveLevel)
				floor[i][j]->Render(dTime, (sinf(waveEffect + (i + j) / (PI / 2))) / 4); 
			else
				floor[i][j]->Render(dTime);

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
		return new Tile(type, x, y, width, pad, anch, true, true);
		break;

	case Tile::TileType::ePickup:
		return new TilePickup(type, x, y, width, pad, anch, false, true);
		break;
	}
}

TileFloor* Level::createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch)
{
	switch (type) {
	default:
		return new TileFloor(type, x, y, width, pad, anch, false, true);
		break;
	}
}

Vector3 Level::move(const Vector3& pos, const Vector2& dir, bool& success)
{
	const Vector2 pPos = getCellFromCoords(pos);
	const Vector2 newPos = pPos + dir;

	success = (newPos.x >= 0 && newPos.x < cellDim && newPos.y >= 0 && newPos.y < cellDim);
	if (success)
	{
		// 'Bump' lets the new tile know that the player wishes to move there, while returning if it's possible
		success = level[newPos.x][newPos.y]->bump() && floor[newPos.x][newPos.y]->bump();

		if (success) {
			// Move off of the previous tile and floor
			floor[pPos.x][pPos.y]->moveOff();
			level[pPos.x][pPos.y]->moveOff();

			// Call any important functions for moving onto a new tile
			floor[newPos.x][newPos.y]->moveOn();
			level[newPos.x][newPos.y]->moveOn();

			return getCoordsFromCell(newPos, pos);
		}
		else
			return pos;
	}
	else
		return pos;
}

float Level::getZOfTile(const Vector3 & pos)
{
	Vector2 location = getCellFromCoords(pos);
	return floor[location.x][location.y]->getPosition().z;
}

Vector2 Level::getCellFromCoords(const Vector3& pos)
{
	return Vector2((pos.y - anchorPos.x) / (tileWidth + tilePadding), (pos.x - anchorPos.y) / (tileWidth + tilePadding));
}

Vector3 Level::getCoordsFromCell(const Vector2& cell, const Vector3& prevPos)
{
	return Vector3(cell.y * (tileWidth + tilePadding) + anchorPos.x, cell.x * (tileWidth + tilePadding) + anchorPos.y, prevPos.z);
}

int Level::getPickupNo() const
{
	return pickupNo_;
}

int Level::getCollectedNo() const
{
	return collectedNo_;
}

void Level::countPickups(const Tile::TileType& layout){
	if (layout == Tile::ePickup){
		pickupNo_++;
	}
}

