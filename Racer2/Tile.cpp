#include <sstream>
#include <thread>
#include <iomanip>

#include "WindowUtils.h"
#include "D3D.h"
#include "Tile.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Tile::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void Tile::Initialise(Mesh& tileMesh)
{
	tile.Initialise(tileMesh);
	if (tileType == eEmpty)
		hidden = true;

	mFX.Init(gd3dDevice);
}

void Tile::Release()
{
	
}

void Tile::Update(float dTime)
{

}


void Tile::Render(float dTime)
{
	if (!hidden)
	{
		mFX.Render(tile, gd3dImmediateContext);

		tile.GetPosition().x = anchor.x + cellX * (cellWidth + padding);
		tile.GetPosition().y = anchor.y + cellY * (cellWidth + padding);
		tile.GetPosition().z = anchor.z + cellWidth + padding;
	}
}

// getters
int Tile::getCellX() {
	return cellX;
}

int Tile::getCellY() {
	return cellY;
}

float Tile::getPadding() {
	return padding;
}
float Tile::getCellWidth() {
	return cellWidth;
}

Vector3 Tile::getAnchor() {
	return anchor;
}
bool Tile::getHidden() {
	return hidden;
}