#include <sstream>
#include <thread>
#include <iomanip>

#include "WindowUtils.h"
#include "D3D.h"
#include "Grid.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Grid::OnResize(int screenWidth, int screenHeight)
{

	OnResize_Default(screenWidth, screenHeight);
}

void Grid::Initialise()
{
	mFX.Init(gd3dDevice);

	//mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	//assert(mpSpriteBatch);
	//mpFont = new SpriteFont(gd3dDevice, L"data/comicSansMS.spritefont");
	//assert(mpFont);

	//mpFont2 = new SpriteFont(gd3dDevice, L"data/algerian.spritefont");
	//assert(mpFont2);

	tileWidth = 1;
	tilePadding = 1.25;

	Mesh& cubeMesh = BuildCube(mMeshMgr);
	for (int i = 0; i < 10; i++) {
		vector<Tile> tempVector;

		for (int j = 0; j < 10; j++) {
			Tile temp();
			tempVector.push_back(Tile(j, i, tileWidth, tilePadding));
			tempVector[j].Initialise(cubeMesh);
		}

		tileArray.push_back(tempVector);
	}
}

void Grid::Release()
{
	mFX.Release();
	mMeshMgr.Release();
	/*delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont2;
	mpFont2 = nullptr;*/
}

void Grid::Update(float dTime)
{
	
}


void Grid::Render(float dTime)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			tileArray[i][j].Render(dTime);
		}

}