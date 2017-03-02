#include <sstream>
#include <thread>
#include <iomanip>

#include "WindowUtils.h"
#include "D3D.h"
#include "Grid.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"
#include "AudioMgr.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Grid::OnResize(int screenWidth, int screenHeight)
{

	OnResize_Default(screenWidth, screenHeight);
}


void Grid::Load()
{

	//Mesh& ecar = mMeshMgr.CreateMesh("ferrari");
	//ecar.CreateFrom("data/ferrari.x", gd3dDevice, mFX.mCache);
	//mCar.Initialise(ecar);
	//mCar.GetPosition() = Vector3(0, -2, 6);
	//mCar.GetMesh().GetSubMesh(1).material.gfxData.Set(Vector4(1,1,1,1), Vector4(1,1,1,1), Vector4(0.125f, 0.125f, 0.05f, 5));  //body has a touch of speculr shinyness
	//mCar.GetMesh().GetSubMesh(0).material.gfxData.Set(Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(0, 0, 0, 1));  //tyres are not shiny!
	//mLoadData.loadedSoFar++;

	Mesh& cubeMesh = BuildCube(mMeshMgr);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			cubeArray[i][j].Initialise(cubeMesh);
			mLoadData.loadedSoFar++;
		}

	//MaterialExt *pMat = &mCube.GetMesh().GetSubMesh(0).material;
	//pMat->gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0, 0, 0, 1));
	//pMat->pTextureRV = mFX.mCache.LoadTexture("floor.dds", true, gd3dDevice);
	//pMat->texture = "floor";
	


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

	mLoadData.totalToLoad = 100;
	mLoadData.loadedSoFar = 0;
	mLoadData.running = true;
	mLoadData.loader = std::async(launch::async, &Grid::Load, this);
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
	if (mLoadData.running)
	{
		if (!mLoadData.loader._Is_ready())
		{
			return;
		}

		mLoadData.loader.get();
		mLoadData.running = false;
		return;
	}


	

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			mFX.Render(cubeArray[i][j], gd3dImmediateContext);
		}

}