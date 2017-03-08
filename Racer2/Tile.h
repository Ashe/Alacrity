#ifndef TILE_H
#define TILE_H

#include <vector>
#include <future>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//wrap up common behaviors, initialization+shutdown
class Tile
{
public:

	enum TileType
	{
		eEmpty, eFloor, eBasic
	};

	//start up and shut down
	Tile(const TileType& type, int x, int y, float width, float pad, const Vector3& anch) 
		:tileType(type), cellX(x), cellY(y), cellWidth(width), padding(pad), anchor(anch), hidden(false) {}

	~Tile() {
		Release();
	}	
	
	//game logic, called constantly, elapsed time passed in
	virtual void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	virtual void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	virtual void Initialise(Mesh& tileMesh);
	virtual void Release();

	// Function to call when you wish to remove the tile from play
	virtual void killTile();

	//game models that reference meshes
	Model tile;

	//handy rendering state
	FX::MyFX mFX;

	// getters
	int getCellX();
	int getCellY();
	Vector3 getAnchor();
	Vector3 getDisplacement();
	float getPadding();
	float getCellWidth();
	bool getHidden();

	TileType getTileType();

	void addDisplacement(const Vector3& disp);
	void resetDisplacement();

private:

	const TileType tileType;

	// attributes for positioning
	const int cellX;
	const int cellY;
	const float cellWidth;
	float padding;
	const Vector3 anchor;

	// vector for displacing the tile
	Vector3 adjustVector;

	// visibility of the tile
	bool hidden;
};

#endif




