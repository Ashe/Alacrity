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
	mUI.setRes(screenWidth, screenHeight);
}

void GameScreen::Initialise()
{

	
	levelMGR.Initialise();
	Mesh& playerMesh = BuildOcta(*mMeshMgr);
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
	
}

int GameScreen::Update(float dTime)
{
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
	
	mUI.DisplayMessage(levelMGR.getMessage(), levelMGR.getTimer(), levelMGR.getLevelMaxTime(), levelMGR.getCollectedNo(), levelMGR.getPickupNo());

	EndRender();
}

void GameScreen::reset(){
	levelMGR.reset();
}