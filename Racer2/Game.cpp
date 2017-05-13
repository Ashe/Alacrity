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
	if (currentScreen != NULL)
		currentScreen->OnResize(screenWidth, screenHeight);
	else
		OnResize_Default(screenWidth, screenHeight);
}


void Game::Load()
{
	// Not even called?
}

void Game::LoadDisplay(float dTime)
{
	BeginRender(Colours::Black);

	static int pips = 0;
	static float elapsed = 0;
	elapsed += dTime;
	EndRender();
}

void Game::Initialise()
{
	mFX.Init(gd3dDevice);

	game = new GameScreen(&mFX);
	game->Initialise();
	currentScreen = game;
}

void Game::Release()
{
	game->Release();
}

void Game::Update(float dTime)
{
	if (mLoadData.running)
		return;

	currentScreen->Update(dTime);
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

	currentScreen->Render(dTime);

}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (currentScreen != NULL)
		return currentScreen->WindowsMssgHandler(hwnd, msg, wParam, lParam);
	else
		return DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

