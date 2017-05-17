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
	menuOption = 1;
	menuOptions = 2;
	upPress = false;
	downPress = false;
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

	if (mMKInput->IsPressed(VK_W)){
		if (!upPress){
			menuOption++;
			upPress = true;
		}
	}
	else if (mMKInput->IsPressed(VK_S)){
		if (!downPress){
			menuOption--;
			downPress = true;
		}
	}
	else{
		upPress = false;
		downPress = false;
	}

	if (menuOption == 0)
		menuOption = menuOptions;
	else if (menuOption > menuOptions)
		menuOption = 1;

	if (mMKInput->IsPressed(VK_SPACE)){
		switch (menuOption){
		case 1:
			return 1;
			break;
		case 2:
			return -1;
			break;
		}
	}

	return 0;
}


void MenuScreen::Render(float dTime)
{
	BeginRender(Colours::Black);

	mpSpriteBatch->Begin();

	wstringstream ssOption1;
	ssOption1 << "Play";
	if (menuOption == 1)
		mpFont->DrawString(mpSpriteBatch, ssOption1.str().c_str(), Vector2(screenCenterX - 25, screenCenterY - 40), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
	else
		mpFont->DrawString(mpSpriteBatch, ssOption1.str().c_str(), Vector2(screenCenterX - 25, screenCenterY - 40), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));


	wstringstream ssOption2;
	ssOption2 << "Quit";
	if (menuOption == 2)
		mpFont->DrawString(mpSpriteBatch, ssOption2.str().c_str(), Vector2(screenCenterX - 25, screenCenterY), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
	else
		mpFont->DrawString(mpSpriteBatch, ssOption2.str().c_str(), Vector2(screenCenterX - 25, screenCenterY), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

	mpSpriteBatch->End();
	EndRender();
}
