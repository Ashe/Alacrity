#ifndef TILE_H
#define TILE_H

#include <vector>
#include <future>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"

//wrap up common behaviors, initialization+shutdown
class Tile
{
public:
	//start up and shut down

	Tile(int x, int y, float width, float pad) 
		:cellX(x), cellY(y), cellWidth(width), padding(pad) {}
	~Tile() {
		Release();
	}
	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	void Initialise(Mesh& tileMesh);
	void Release();

	//ensure each mesh is loaded and release exactly once
	MeshManager mMeshMgr;

	//game models that reference meshes
	//Model mCube;
	Model tile;

	//handy rendering state
	FX::MyFX mFX;

private:

	// attributes for positioning
	const int cellX;
	const int cellY;
	const float cellWidth;
	const float padding;
};

#endif




