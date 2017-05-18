#include <sstream>
#include <thread>
#include <iomanip>

#include "GameScreen.h"

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"
#include "AudioMgr.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void GameScreen::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
}

void GameScreen::LoadDisplay(float dTime)
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

void GameScreen::Initialise()
{
	levelMGR.Initialise();
	Mesh& playerMesh = BuildSphere(*mMeshMgr, 20,20);
	player.Initialise(mFX, playerMesh, &levelMGR, mMKInput);

	mQuad.Initialise(BuildQuad(*mMeshMgr));
	MaterialExt *pMat = &mQuad.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0, 0, 0, 1));
	pMat->pTextureRV = mFX->mCache.LoadTexture("gamebgd.dds", true, gd3dDevice);
	pMat->texture = "gamebgd";

	
	FX::SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, -0.7f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(1, 1, 1));

	mUI.Initialise();

	mCamPos = mDefCamPos;
}

void GameScreen::Release()
{
	levelMGR.Release();
	player.Release();

	mFX->Release();
	mUI.Release();

	/*delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont2;
	mpFont2 = nullptr;*/
}

int GameScreen::Update(float dTime)
{
	GetIAudioMgr()->Update();

	levelMGR.Update(dTime);
	player.Update(dTime);

	if (mMKInput->IsPressed(VK_ESCAPE) || mMKInput->IsPressed(VK_Q))
		return 1;

	return 0;
}


void GameScreen::Render(float dTime)
{
	BeginRender(Colours::Black);

	levelMGR.Render(dTime);
	player.Render(dTime);

	FX::SetPerFrameConsts(gd3dImmediateContext, mCamPos);

	CreateProjectionMatrix(FX::GetProjectionMatrix(), 0.25f*PI, GetAspectRatio(), 1, 1000.f);
	CreateViewMatrix(FX::GetViewMatrix(), Vector3(-25, -15, 25), Vector3(0, 0, 0), Vector3(0, 0, 1));

	mQuad.GetPosition() = Vector3(100, 60, -40);
	mQuad.GetRotation() = Vector3(PI / 2, 0, PI / 4);
	mQuad.GetScale() = Vector3(150, 1, 150);
	mFX->Render(mQuad, gd3dImmediateContext);

	CommonStates state(gd3dDevice);
	
	mUI.DisplayMessage(levelMGR.getMessage(), levelMGR.getTimer(), levelMGR.getCollectedNo(), levelMGR.getPickupNo());

	EndRender();
}
