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
	screenCenterX = screenWidth / 2;
	screenCenterY = screenHeight / 2;
}

void MenuScreen::Initialise()
{
	SeedRandom();

	levelMGR.Initialise(5);
	menuTimer.setTimer(10);
	menuTimer.startTimer();

	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);
	mpFont = new SpriteFont(gd3dDevice, L"data/cabin.spritefont");
	assert(mpFont);
	screenCenterX = 512;
	screenCenterY = 384;
	menuOption = 1;
	menuOptions = 3;
	upPress = false;
	downPress = false;
	viewCredits = false;
	canResetB = false;
	isResetB = false;
}

void MenuScreen::Release()
{
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;

	levelMGR.Release();
}

int MenuScreen::Update(float dTime)
{
	GetIAudioMgr()->Update();
	levelMGR.Update(dTime);

	menuTimer.updateTimer(dTime);
	if (menuTimer.getTimer() <= 0) {
		levelMGR.forceRandomLevel();
		menuTimer.resetTimer();
		menuTimer.startTimer();
	}

	if (viewCredits){
		if (mMKInput->IsPressed(VK_Q))
			viewCredits = false;
	}else if (canResetB){
		menuInputs();
		if (mMKInput->IsPressed(VK_SPACE)){
			switch (menuOption){
			case 1:
				canReset();
				return 1;
				break;
			case 2:
				isResetB = true;
				return 1;
				break;
			case 3:
				viewCredits = true;
				break;
			case 4:
				return -1;
				break;
			}
		}
	}else{
		menuInputs();
		if (mMKInput->IsPressed(VK_SPACE)){
			switch (menuOption){
			case 1:
				canReset();
				return 1;
				break;
			case 2:
				viewCredits = true;
				break;
			case 3:
				return -1;
				break;
			}
		}
	}

	return 0;
}


void MenuScreen::Render(float dTime)
{
	// Camera angle calculations
	mAngle += dTime * mAngleSpeed;
	mCamPos.x = camRadHeight.x * cos(mAngle);
	mCamPos.y = camRadHeight.x * sin(mAngle);
	mCamPos.z = camRadHeight.y;

	BeginRender(Colours::Black);

	levelMGR.Render(dTime);

	FX::SetPerFrameConsts(gd3dImmediateContext, mCamPos);

	CreateProjectionMatrix(FX::GetProjectionMatrix(), 0.25f*PI, GetAspectRatio(), 1, 1000.f);
	CreateViewMatrix(FX::GetViewMatrix(), mCamPos, Vector3(0, 0, 0), Vector3(0, 0, 1));

	mpSpriteBatch->Begin();

	if (viewCredits){
		wstringstream ssCredit1;
		wstringstream ssCredit2;
		wstringstream ssCredit3;
		wstringstream ssBack;
		ssCredit1 << "Scrum Master: Holly Savin";
		ssCredit2 << "Behaviour Programmer: Ashley Smith";
		ssCredit3 << "Player Programmer: Elliott Hill";
		ssBack << "(Q) Back";
		mpFont->DrawString(mpSpriteBatch, ssCredit1.str().c_str(), Vector2(screenCenterX - 170, screenCenterY - 60), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		mpFont->DrawString(mpSpriteBatch, ssCredit2.str().c_str(), Vector2(screenCenterX - 250, screenCenterY - 20), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		mpFont->DrawString(mpSpriteBatch, ssCredit3.str().c_str(), Vector2(screenCenterX - 200, screenCenterY + 20), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		mpFont->DrawString(mpSpriteBatch, ssBack.str().c_str(), Vector2(5, screenCenterY * 2 - 40), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
	}
	else{
		wstringstream ssOption1;
		wstringstream ssOption2;
		wstringstream ssOption3;
		wstringstream ssOption4;
		int posOption1x, posOption2x, posOption3x, posOption4x, posOption1y, posOption2y, posOption3y, posOption4y;
		if (canResetB){
			ssOption1 << "Continue";
			ssOption2 << "Reset";
			ssOption3 << "Credits";
			ssOption4 << "Quit";
			posOption1x = -55;
			posOption2x = -30;
			posOption3x = -40;
			posOption4x = -25;
			posOption1y = -100;
			posOption2y = -60;
			posOption3y = -20;
			posOption4y = 20;
		}else{
			ssOption1 << "Play";
			ssOption2 << "Credits";
			ssOption3 << "Quit";
			posOption1x = -25;
			posOption2x = -40;
			posOption3x = -25;
			posOption1y = -60;
			posOption2y = -20;
			posOption3y = 20;
		}
		if (menuOption == 1)
			mpFont->DrawString(mpSpriteBatch, ssOption1.str().c_str(), Vector2(screenCenterX + posOption1x, screenCenterY + posOption1y), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		else
			mpFont->DrawString(mpSpriteBatch, ssOption1.str().c_str(), Vector2(screenCenterX + posOption1x, screenCenterY + posOption1y), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

		if (menuOption == 2)
			mpFont->DrawString(mpSpriteBatch, ssOption2.str().c_str(), Vector2(screenCenterX + posOption2x, screenCenterY + posOption2y), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		else
			mpFont->DrawString(mpSpriteBatch, ssOption2.str().c_str(), Vector2(screenCenterX + posOption2x, screenCenterY + posOption2y), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

		if (menuOption == 3)
			mpFont->DrawString(mpSpriteBatch, ssOption3.str().c_str(), Vector2(screenCenterX + posOption3x, screenCenterY + posOption3y), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		else
			mpFont->DrawString(mpSpriteBatch, ssOption3.str().c_str(), Vector2(screenCenterX + posOption3x, screenCenterY + posOption3y), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

		if (canResetB){
			if (menuOption == 4)
				mpFont->DrawString(mpSpriteBatch, ssOption4.str().c_str(), Vector2(screenCenterX + posOption4x, screenCenterY + posOption4y), Colours::Blue, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
			else
				mpFont->DrawString(mpSpriteBatch, ssOption4.str().c_str(), Vector2(screenCenterX + posOption4x, screenCenterY + posOption4y), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
		}
		wstringstream ssSpace;
		ssSpace << "(SPACE) Select";
		mpFont->DrawString(mpSpriteBatch, ssSpace.str().c_str(), Vector2(5, screenCenterY * 2 - 40), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
	}

	mpSpriteBatch->End();

	EndRender();
}

void MenuScreen::menuInputs(){
	if (mMKInput->IsPressed(VK_W)){
		if (!upPress){
			menuOption--;
			upPress = true;
		}
	}
	else if (mMKInput->IsPressed(VK_S)){
		if (!downPress){
			menuOption++;
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
}

void MenuScreen::canReset(){
	canResetB = true;
	menuOptions = 4;
}

bool MenuScreen::isReset() const{
	return isResetB;
}

void MenuScreen::setIsReset(const bool state){
	isResetB = state;
}