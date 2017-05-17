#include <sstream>
#include <thread>
#include <iomanip>

#include "MenuScreen.h"

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"
#include "AudioMgr.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void MenuScreen::OnResize(int screenWidth, int screenHeight)
{
	OnResize_Default(screenWidth, screenHeight);
	screenCenterX = screenWidth / 2;
	screenCenterY = screenHeight / 2;
}

void MenuScreen::Initialise()
{
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);
	mpFont = new SpriteFont(gd3dDevice, L"data/cabin.spritefont");
	assert(mpFont);
	screenCenterX = 512;
	screenCenterY = 384;
}

void MenuScreen::Release()
{
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
}

int MenuScreen::Update(float dTime)
{
	GetIAudioMgr()->Update();



	if (mMKInput->IsPressed(VK_SPACE)) {
		playerHoldingQuit = true;
		return 1;
	}

	bool playerWantsToQuit = (mMKInput->IsPressed(VK_ESCAPE) || mMKInput->IsPressed(VK_Q));
	if (!playerHoldingQuit && playerWantsToQuit)
		return -1;

	playerHoldingQuit = playerWantsToQuit;



	return 0;
}


void MenuScreen::Render(float dTime)
{
	BeginRender(Colours::Black);

	mpSpriteBatch->Begin();

	wstringstream ssMessage;
	ssMessage << "Press Space to play";
	mpFont->DrawString(mpSpriteBatch, ssMessage.str().c_str(), Vector2(screenCenterX - 125, screenCenterY - 40), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));


	wstringstream ssTimer;
	ssTimer << "Press Escape to quit";
	mpFont->DrawString(mpSpriteBatch, ssTimer.str().c_str(), Vector2(screenCenterX - 130, screenCenterY), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

	mpSpriteBatch->End();
	EndRender();
}
