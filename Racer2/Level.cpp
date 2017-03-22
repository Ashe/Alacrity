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
	static Mesh& tempMesh = BuildCube(mMeshMgr);

	pickupNo_ = 0;

	mFX.Init(gd3dDevice);

	// Set level specific params
	cellDim = dim;
	tilePadding = padding;

	maxAdditionalPadding = 30;
	levelTransitionSpeed = 16;
	additionalPadding = maxAdditionalPadding;

	levelReadyToPlay = false;
	playerInPlay = false;
	playerReachedGoal = false;
	readyToTransition = false;

	Release();

	// Initialise both foreground and background
	for (int i = 0; i < dim; i++) {
		vector<Tile*> tempLevel;
		vector<TileFloor*> tempBack;

		for (int j = 0; j < dim; j++) {
			// Create and intialise a floor tile and place it on the background grid
			tempBack.push_back(createFloorTile(layout[i][j], j, i, tileWidth, tilePadding, anchorPos));
			tempBack[j]->Initialise(tempMesh);// meshArray[1]);

			// Create and initialise a tile given in the layout, created in the levelMGR
			tempLevel.push_back(createTile(layout[i][j],j, i, tileWidth,tilePadding,anchorPos));
			tempLevel[j]->Initialise(tempMesh);// meshArray[layout[i][j]]);

			countPickups(layout[i][j]);
		}

		// Add the row of tiles to the level's storage and repeat
		floor.push_back(tempBack);
		level.push_back(tempLevel);
	}

	// Fancy effects
	waveEffect = 0;
	waveLevel = true;

	checkLevel();
}

void Level::Release()
{
	for (int i = 0; i < floor.size(); i++)
		for (int j = 0; j < floor.size(); j++)
		{
			delete floor[i][j];
			delete level[i][j];
		}

	floor.clear();
	level.clear();
}

void Level::Update(float dTime)
{
	checkGameState(dTime);

	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			floor[i][j]->Update(dTime, tilePadding + additionalPadding);  
			level[i][j]->Update(dTime, tilePadding + additionalPadding);
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

	case Tile::TileType::eStart:
		return new TileStart(type, x, y, width, pad, anch, false, true);
		break;

	case Tile::TileType::eEnd:
		return new TileEnd(type, x, y, width, pad, anch, false, true);
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
	if (levelReadyToPlay)
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

				// Check the state of the game
				checkLevel();

				return getCoordsFromCell(newPos, pos);
			}
			else
				return pos;
		}
		else
			return pos;
	}
	else
		return pos;
}

Vector3 Level::getStartingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eStart)
				return getCoordsFromCell(Vector2(i, j));
	}

	// If this function gets this far, there is no starting tile and MUST be fixed
	assert(false);
}

Vector3 Level::getEndingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eEnd)
				return getCoordsFromCell(Vector2(i, j));
	}

	// If this function gets this far, there is no starting tile and MUST be fixed
	assert(false);
}

Vector3 Level::getCurrentLocationOfTile(const Vector3 & pos)
{
	Vector3 pPos;

	if (playerInPlay) {
		const Vector3 temp = { std::floor(pos.x * 100) / 100, std::floor(pos.y * 100) / 100, pos.z };
		const Vector2 v2 = getCellFromCoords(temp);
		pPos = getCoordsFromCell(v2);
	}
	else if (playerReachedGoal)
		pPos = getEndingPosition();
	else
		pPos = getStartingPosition();

	pPos.z += tilePadding + additionalPadding;
	return pPos;
}

Vector2 Level::getCellFromCoords(const Vector3& pos) const
{
	//return Vector2(std::floor((pos.y - anchorPos.x) / (tileWidth + tilePadding + additionalPadding)), std::floor((pos.x - anchorPos.y) / (tileWidth + tilePadding + additionalPadding)));
	return Vector2(std::ceil((pos.y - anchorPos.x) / (tileWidth + tilePadding + additionalPadding)) + 5, std::ceil((pos.x - anchorPos.y) / (tileWidth + tilePadding + additionalPadding)) + 5);
}

Vector3 Level::getCoordsFromCell(const Vector2& cell, const Vector3& prevPos) const
{
	return Vector3((cell.y - 5) * (tileWidth + tilePadding + additionalPadding) + anchorPos.x, (cell.x - 5) * (tileWidth + tilePadding + additionalPadding) + anchorPos.y, (sinf(waveEffect + (cell.x + cell.y) / (PI / 2))) / 4);
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

void Level::checkLevel()
{
	collectedNo_ = 0;

	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {
			checkTile(level[i][j]);
		}
}

void Level::checkTile(Tile* tile)
{
	// Count how many pickups have been collected
	if (tile->getTileType() == Tile::ePickup && tile->getInfo()) {
		collectedNo_++;
	}
	// Check if the player has begun the level
	else if (tile->getTileType() == Tile::eStart) {
		playerInPlay = tile->getInfo();
	}
	// Check if the player has reached the end
	else if (tile->getTileType() == Tile::eEnd) {
		playerReachedGoal = tile->getInfo();
	}
}

void Level::checkGameState(float dTime)
{
	if (playerReachedGoal) {
		levelReadyToPlay = false;

		if (additionalPadding >= maxAdditionalPadding) {
			additionalPadding = maxAdditionalPadding;
			readyToTransition = true;
			playerReachedGoal = false;
		}

		additionalPadding += dTime * levelTransitionSpeed;
	}
	else if (!levelReadyToPlay)
	{
		if (additionalPadding <= 0) {
			additionalPadding = 0;
			levelReadyToPlay = true;
		}

		additionalPadding -= dTime * levelTransitionSpeed;
	}
}

bool Level::getLevelStarted() const
{
	return playerInPlay;
}

bool Level::getLevelEnded() const
{
	return playerReachedGoal;
}

bool Level::getLevelSwitch() const
{
	return readyToTransition;
}

