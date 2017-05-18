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
	if (menu != NULL)
		menu->OnResize(screenWidth, screenHeight);

	if (game != NULL)
		game->OnResize(screenWidth, screenHeight);

	OnResize_Default(screenWidth, screenHeight);
}

void Game::Initialise()
{
	GetIAudioMgr()->Initialise();
	mFX.Init(gd3dDevice);
	mMK.Initialise(GetMainWnd());

	// Build a cube for use in both levels
	BuildCube(mMM);

	game->Initialise();
	menu->Initialise();

	currentScreen = menu;
}

void Game::Release()
{
	game->Release();
	menu->Release();
	mMM.Release();
}

void Game::Update(float dTime)
{

	GetIAudioMgr()->Update();

	int state = currentScreen->Update(dTime);
	if (state == -1) {
		PostQuitMessage(0);
		return;
	}
	else if (state == 1)
		if (currentScreen == game){
			currentScreen = menu;
			if (GetIAudioMgr()->GetSongMgr()->IsPlaying(mMusicHdl))
				GetIAudioMgr()->GetSongMgr()->SetPause(true, mMusicHdl);
		}
		else{
			currentScreen = game;
			GetIAudioMgr()->GetSongMgr()->SetPause(false, mMusicHdl);
		}

		if (!GetIAudioMgr()->GetSongMgr()->IsPlaying(mMusicHdl))
			GetIAudioMgr()->GetSongMgr()->Play("song", true, false, &mMusicHdl, 0.2f);

		if (menu->isReset()){
			menu->setIsReset(false);
			game->reset();
		}
	
}


void Game::Render(float dTime)
{
	currentScreen->Render(dTime);

	mMK.PostProcess();
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 20.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		/*switch (wParam)
		{
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		}*/
	case WM_INPUT:
		mMK.MessageEvent((HRAWINPUT)lParam);
		break;
	}
	//default message handling (resize window, full screen, etc)
	return DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

