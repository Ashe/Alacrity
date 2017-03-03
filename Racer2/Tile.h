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
	//start up and shut down
	Tile(int x, int y, float width, float pad, const Vector3& anch, bool hide = 0) 
		:cellX(x), cellY(y), cellWidth(width), padding(pad), anchor(anch), hidden(hide) {}
	~Tile() {
		Release();
	}	
	
	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	virtual void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	void Initialise(Mesh& tileMesh);
	void Release();

	//game models that reference meshes
	Model tile;

	//handy rendering state
	FX::MyFX mFX;

	// getters
	int getCellX();
	int getCellY();
	Vector3 getAnchor();
	float getPadding();
	float getCellWidth();
	bool getHidden();

private:

	// attributes for positioning
	const int cellX;
	const int cellY;
	const float cellWidth;
	const float padding;
	const Vector3 anchor;
	const bool hidden;
};

#endif




