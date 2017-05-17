#include "Level.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Level::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Level::Initialise(const TextureInfo& texInf, const vector<vector<Tile::TileType>>& layout, const vector<vector<int>>& floorLayout, const vector<vector<int>>& extraInfo, const string& caption, float levelTime, float width, float padding, int dim, float safeTime, float fallSpeedSafe, float fallSpeedDead)
{
	// Pause any animations
	levelFinishedLoading = false;

	// Import references to the textures
	textureInfo = texInf;

	// Set level specific params
	tileWidth = width;
	cellDim = dim;
	tilePadding = padding;
	levelCaption = caption;

	// Set values for the 'explosive' transitions
	maxAdditionalPadding = 40;
	levelTransitionSpeed = 26;
	additionalPadding = maxAdditionalPadding;

	// Initialise bools to control the flow of the game
	levelReadyToPlay = false;
	playerInPlay = false;
	endTheGame = false;
	playerBeatLevel = false;
	readyToTransition = false;
	playerFrozen = false;

	// In prep for counting pickups
	pickupNo_ = 0;
	trueCollectedNo_ = 0;

	// Before initialising the next level, release anything stored in tile vectors
	Release();

	// Initialise both foreground and background
	for (int i = 0; i < dim; i++) {
		vector<Tile*> tempLevel;
		vector<TileFloor*> tempBack;

		for (int j = 0; j < dim; j++) {
			// Create and intialise tiles and place it in the appropriate vector
			tempBack.push_back(createFloorTile(layout[i][j], j, i, tileWidth, tilePadding + additionalPadding, anchorPos, safeTime, fallSpeedSafe, fallSpeedDead, floorLayout[i][j]));
			tempLevel.push_back(createTile(layout[i][j],j, i, tileWidth,tilePadding,anchorPos, extraInfo[i][j]));

			countPickups(layout[i][j]);
		}

		// Add the row of tiles to the level's storage and repeat
		floor.push_back(tempBack);
		level.push_back(tempLevel);
	}

	// Fancy effects
	waveEffect = 0;
	waveLevel = true;

	// Allow the level to animate
	levelFinishedLoading = true;

	timer.pauseTimer();
	levelMaxTime = levelTime;
	timer.setTimer(levelMaxTime);
}

void Level::Release()
{
	for (int i = 0; i < floor.size(); i++)
	{
		for (int j = 0; j < floor.size(); j++)
		{
			// Delete dynamically allocated memory
			delete floor[i][j];
			delete level[i][j];
		}
		// Ensure that the sub-vector is cleared of any pointers
		floor[i].clear();
		level[i].clear();
	}

	// Finally, empty the main vectors
	floor.clear();
	level.clear();
}

void Level::Update(float dTime)
{
	// If the tiles have finished generating
	if (levelFinishedLoading)
	{
		// Updates the timer for later use
		timer.updateTimer(dTime);
		if (timer.getTimer() <= 0)
			endTheGame = true;

		// Transitions in/out depending on game conditions and resets amount collected
		checkGameState(dTime);
		collectedNo_ = 0;

		// Iterate through the 2D array, updating and then checking their info
		for (int i = 0; i < cellDim; i++)
			for (int j = 0; j < cellDim; j++) {
				floor[i][j]->Update(dTime, tilePadding + additionalPadding);  
				level[i][j]->Update(dTime, tilePadding + additionalPadding);

				// These functions check tile-types and acts on info gathered
				checkTileFloor(floor[i][j]);
				checkTile(level[i][j]);
			}

		if (collectedNo_ > trueCollectedNo_)
			trueCollectedNo_ = collectedNo_;
	}
}

void Level::Render(float dTime)
{
	// If the level is set to wave, animate it
	if (waveLevel)
		waveEffect += dTime * 1.5;
	
	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {

			if (waveLevel)
				floor[i][j]->Render(dTime, (sinf(waveEffect + (i + j) / (PI / 2))) / 4); 
			else
				floor[i][j]->Render(dTime);

			// The main level uses the floor's positioning
			level[i][j]->Render(dTime, floor[i][j]->getPosition().z, true);
		}
}

void Level::Load(MeshManager meshMGR)
{
	// TODO: Load meshes from an array to coincide with ENUM
	// Turns out this function is no longer needed, but will keep it in case it's needed later
}

Tile* Level::createTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch, int info)
{
	// Declare the pointer that will be returned later
	Tile* peter;
	Mesh* randy = mMeshMgr->GetMesh("cube");


	// Depending on the type of tile, create an instance of the appropriate class
	switch (type) {
	case Tile::TileType::eBasic:
		peter = new Tile(*mFX, type, x, y, width, pad, anch, cellDim);
		break;

	case Tile::TileType::eEmpty:
		peter = new Tile(*mFX, type, x, y, width, pad, anch, cellDim, true, true);
		break;

	case Tile::TileType::ePikup:
		peter = new TilePickup(*mFX, type, x, y, width, pad, anch, cellDim, false, true, info);
		break;

	case Tile::TileType::eInviW:
		peter = new TileInvisibleWall(*mFX, type, x, y, width, pad, anch, cellDim);
		break;

	case Tile::TileType::eFakeW:
		peter = new TileInverseInvisibleWall(*mFX, type, x, y, width, pad, anch, cellDim);
		break;

	case Tile::TileType::eStart:
		peter = new TileStart(*mFX, type, x, y, width, pad, anch, cellDim, false, true);
		break;

	case Tile::TileType::eEnder:
		peter = new TileEnd(*mFX, type, x, y, width, pad, anch, cellDim, false, true);
		break;
	}
	
	// Initialise the tile with the mesh in randy reference
	peter->Initialise(*randy);
	return peter;
}

TileFloor* Level::createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const Vector3& anch, float safeTime, float fallSpeedSafe, float fallSpeedDead, int protection)
{
	// Declare the pointer that will be returned later
	TileFloor* peter;
	Mesh* randy = mMeshMgr->GetMesh("cube");

	// Depending on the floor tile specified, create an appropriate floor (For now, just spawn the default floor)
	switch (type) {
	default:
		peter = new TileFloor(*mFX, type, x, y, width, pad, anch, cellDim, safeTime, fallSpeedSafe, fallSpeedDead, false, true, protection);
		break;
	}

	// Return the initialised tile
	peter->Initialise(*randy);
	return peter;
}

// This function receives a position and a direction and returns the location the player should be along with if it was successful
Vector2 Level::move(const Vector2& pos, const Vector2& dir, bool& success)
{
	// If the player is able to move
	if (levelReadyToPlay && !playerFrozen && !endTheGame)
	{
		// Get the cell coordinates of the player's current and next position
		//const Vector2 pPos = getCellFromCoords(pos);
		const Vector2 newPos = pos + dir;

		success = (newPos.x >= 0 && newPos.x < cellDim && newPos.y >= 0 && newPos.y < cellDim);
		if (success)
		{
			// 'Bump' lets the new tile know that the player wishes to move there, while returning if it's possible
			success = level[newPos.x][newPos.y]->bump() && floor[newPos.x][newPos.y]->bump();

			if (success) {
				// Move off of the previous tile and floor
				floor[pos.x][pos.y]->moveOff();
				level[pos.x][pos.y]->moveOff();

				// Call any important functions for moving onto a new tile
				floor[newPos.x][newPos.y]->moveOn();
				level[newPos.x][newPos.y]->moveOn();

				return newPos;
			}
		}
	}

	return (pos);
}

Vector2 Level::getStartingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eStart)
				return Vector2(i, j);
	}

	// If this function gets this far, there is no starting tile and MUST be fixed
	assert(false);
}

Vector3 Level::getEndingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eEnder)
				return getCoordsFromCell(Vector2(i, j));
	}

	// If this function gets this far, there is no ending tile and MUST be fixed
	assert(false);
}

// This function takes the player's cell coords and returns the 3D coords for the player
// to position to. If the Game hasn't started yet, the player is forced to go to the start
// (hence why not a constant function). Helpful when external forces want to move the player.
// A forced check occurs when the player isn't in game so that they aren't moved back to the start,
// otherwise the first move isn't checked until the update call which is after this, resulting in
// the player not leaving the starting tile.
Vector3 Level::getCurrentLocationOfTile(Vector2 & pos)
{
	// Easy out for if the player is playing
	if (endTheGame || playerInPlay)
		return getCoordsFromCell(pos);

	// If the game hasn't started, adjust the player's coordinates before returning
	Vector2 start = getStartingPosition();
	checkTile(level[start.x][start.y]);

	// Forced check so that player isn't moved back to start
	if (!endTheGame && !playerInPlay)
		pos = start;

	// Now that the player is positioned correctly, return
	return getCoordsFromCell(pos);
}

float Level::getTimer() const
{
	return timer.getTimer();
}

//Vector2 Level::getCellFromCoords(const Vector3& pos) const
//{
//	return Vector2(std::ceil((pos.y - anchorPos.x) / (tileWidth + tilePadding + additionalPadding)) + cellDim / 2, std::ceil((pos.x - anchorPos.y) / (tileWidth + tilePadding + additionalPadding)) + cellDim / 2);
//}

Vector3 Level::getCoordsFromCell(const Vector2& cell) const
{
	return Vector3((cell.y - cellDim / 2) * (tileWidth + tilePadding + additionalPadding) + anchorPos.x, (cell.x - cellDim / 2) * (tileWidth + tilePadding + additionalPadding) + anchorPos.y, floor[cell.x][cell.y]->getPosition().z);
}

int Level::getPickupNo() const
{
	return pickupNo_;
}

int Level::getCollectedNo() const
{
	return trueCollectedNo_;
}

void Level::countPickups(const Tile::TileType& layout){
	if (layout == Tile::ePikup){
		pickupNo_++;
	}
}

void Level::checkTile(Tile* tile)
{
	// BLOCKS:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Count how many pickups have been collected
	if (tile->getTileType() == Tile::ePikup) {
		if (tile->getInfo(trueCollectedNo_))
			collectedNo_++;
	}
	// Check if the player has begun the level
	else if (tile->getTileType() == Tile::eStart) {

		if (!playerInPlay && tile->getInfo()) {
			timer.startTimer();
			playerInPlay = true;
		}
	}
	// Check if the player has reached the end
	else if (tile->getTileType() == Tile::eEnder) {

		if (tile->getInfo() && (trueCollectedNo_ >= pickupNo_)) {
			timer.pauseTimer();
			playerBeatLevel = true;
			endTheGame = true;
		}
	}
}

void Level::checkTileFloor(TileFloor* tile)
{
	// FLOORS:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Check if the floor tiles are killing the player
	const int status = tile->getInfo();
	if (status > 0)
	{
		timer.pauseTimer();

		if (status == 1)
			playerFrozen = true;
		else
			endTheGame = true;
	}
}

void Level::checkGameState(float dTime)
{
	// Depending on if the game is over or starting, explode (or implode) the level
	if (endTheGame) {
		levelReadyToPlay = false;

		if (additionalPadding >= maxAdditionalPadding) {
			additionalPadding = maxAdditionalPadding;
			readyToTransition = true;
			endTheGame = false;
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
	return endTheGame;
}

bool Level::getLevelSwitch() const
{
	return readyToTransition;
}

bool Level::getWinStatus() const
{
	return playerBeatLevel;
}

string Level::getMessage() const
{
	if (!endTheGame)
		return levelCaption;

	else if (endTheGame && playerBeatLevel) {
		const float time = levelMaxTime - timer.getTimer();
		const string temp = std::to_string(time);
		return "Congratulations! You beat the level in " + temp + " seconds!";
	}
	else
		return "Try again!";
}

