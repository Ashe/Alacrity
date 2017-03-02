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

	mFX.Init(gd3dDevice);
}

void Tile::Release()
{
	mFX.Release();
	mMeshMgr.Release();
}

void Tile::Update(float dTime)
{

}


void Tile::Render(float dTime)
{
	mFX.Render(tile, gd3dImmediateContext);
	tile.GetPosition().x = cellX * (cellWidth + padding);
	tile.GetPosition().y = cellY * (cellWidth + padding);
}