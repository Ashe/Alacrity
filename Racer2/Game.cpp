#include <sstream>
#include <thread>
#include <iomanip>

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"
#include "AudioMgr.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Game::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}


void Game::Load()
{
	//GetIAudioMgr()->Initialise();
	mLoadData.loadedSoFar++;

	//Mesh& ecar = mMeshMgr.CreateMesh("ferrari");
	//ecar.CreateFrom("data/ferrari.x", gd3dDevice, mFX.mCache);
	//mCar.Initialise(ecar);
	//mCar.GetPosition() = Vector3(0, -2, 6);
	//mCar.GetMesh().GetSubMesh(1).material.gfxData.Set(Vector4(1,1,1,1), Vector4(1,1,1,1), Vector4(0.125f, 0.125f, 0.05f, 5));  //body has a touch of speculr shinyness
	//mCar.GetMesh().GetSubMesh(0).material.gfxData.Set(Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(0, 0, 0, 1));  //tyres are not shiny!
	//mLoadData.loadedSoFar++;

	//wood floor



	//MaterialExt *pMat = &mCube.GetMesh().GetSubMesh(0).material;
	//pMat->gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0, 0, 0, 1));
	//pMat->pTextureRV = mFX.mCache.LoadTexture("floor.dds", true, gd3dDevice);
	//pMat->texture = "floor";
	mLoadData.loadedSoFar++;


}

void Game::LoadDisplay(float dTime)
{
	BeginRender(Colours::Black);

	//mpSpriteBatch->Begin();

	static int pips = 0;
	static float elapsed = 0;
	elapsed += dTime;

	//if (elapsed > 0.25f){
	//	pips++;
	//	elapsed = 0;
	//}
	//if (pips > 10)
	//	pips = 0;
	//wstringstream ss;
	//ss << L"Loading meshes(" << (int)(((float)mLoadData.loadedSoFar / (float)mLoadData.totalToLoad)*100.f) << L"%) ";
	//for (int i = 0; i < pips; ++i)
	//	ss << L'.';
	//mpFont2->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(100, 200), Colours::White, 0, Vector2(0, 0), Vector2(1.f, 1.f));

	//ss.str(L"");
	//ss << L"FPS:" << (int)(1.f / dTime);
	//mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(10, 550), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));

	//mpSpriteBatch->End();

	

	EndRender();
}

void Game::Initialise()
{
	mFX.Init(gd3dDevice);
	levelMGR.Initialise();
	Mesh& playerMesh = BuildPyramid(mMeshMgr);
	player.Initialise(mFX, playerMesh, &levelMGR, &mMKInput);

	mQuad.Initialise(BuildQuad(mMeshMgr));
	MaterialExt *pMat = &mQuad.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0, 0, 0, 1));
	pMat->pTextureRV = mFX.mCache.LoadTexture("gamebgd.dds", true, gd3dDevice);
	pMat->texture = "gamebgd";


	FX::SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, -0.7f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(1, 1, 1));

	//mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	//assert(mpSpriteBatch);
	//mpFont = new SpriteFont(gd3dDevice, L"data/comicSansMS.spritefont");
	//assert(mpFont);

	//mpFont2 = new SpriteFont(gd3dDevice, L"data/algerian.spritefont");
	//assert(mpFont2);

	mUI.Initialise();

	mLoadData.totalToLoad = 2;
	mLoadData.loadedSoFar = 0;
	mLoadData.running = true;
	mLoadData.loader = std::async(launch::async, &Game::Load, this);

	mMKInput.Initialise(GetMainWnd());


	// May want to delete this
	mCamPos = mDefCamPos;
}

void Game::Release()
{
	levelMGR.Release();
	player.Release();

	mFX.Release();
	mMeshMgr.Release();
	mUI.Release();

	/*delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont2;
	mpFont2 = nullptr;*/
}

void Game::Update(float dTime)
{
	GetIAudioMgr()->Update();

	levelMGR.Update(dTime);
	player.Update(dTime);

	if (mLoadData.running)
		return;
}


void Game::Render(float dTime)
{
	if (mLoadData.running)
	{
		if (!mLoadData.loader._Is_ready())
		{
			LoadDisplay(dTime);
			return;
		}
		mLoadData.loader.get();
		mLoadData.running = false;
		return;
	}

	BeginRender(Colours::Black);

	levelMGR.Render(dTime);

	player.Render(dTime);

	

	FX::SetPerFrameConsts(gd3dImmediateContext, mCamPos);

	CreateProjectionMatrix(FX::GetProjectionMatrix(), 0.25f*PI, GetAspectRatio(), 1, 1000.f);


	CreateViewMatrix(FX::GetViewMatrix(), Vector3(-25,-15, 25), Vector3(0, 0, 0), Vector3(0, 0, 1));

	//mFX.Render(mCar, gd3dImmediateContext);
	mQuad.GetPosition() = Vector3(100, 60, -40);
	mQuad.GetRotation() = Vector3(PI / 2, 0, PI/4);
	mQuad.GetScale() = Vector3(150, 1, 150);
	mFX.Render(mQuad, gd3dImmediateContext);

	CommonStates state(gd3dDevice);
	//mpSpriteBatch->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());

	////general messages
	//wstringstream ss;
	//if (dTime > 0)
	//	ss << L"FPS: " << (int)(1.f / dTime);
	//else
	//	ss << L"FPS: 0";
	//mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(10, 550), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));

	//mpSpriteBatch->End();
	mUI.DisplayMessage(levelMGR.getMessage(), levelMGR.getTimer(), levelMGR.getCollectedNo(), levelMGR.getPickupNo());

	EndRender();

	mMKInput.PostProcess();
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 20.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		case ' ':
			mCamPos = mDefCamPos;
			break;
		}
	case WM_INPUT:
			mMKInput.MessageEvent((HRAWINPUT)lParam);
			break;
	}
	//default message handling (resize window, full screen, etc)
	return DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

