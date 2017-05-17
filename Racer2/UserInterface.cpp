#include <iomanip>
#include <sstream>

#include "WindowUtils.h" 
#include "D3D.h"
#include "UserInterface.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void UserInterface::Initialise(){
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);
	mpFont = new SpriteFont(gd3dDevice, L"data/cabin.spritefont");
	assert(mpFont);
}

void UserInterface::DisplayMessage(string& message, float timer, int currentPickUps, int pickupTotal){
	mpSpriteBatch->Begin();

	wstringstream ssMessage;
	ssMessage << message.c_str(); 
	mpFont->DrawString(mpSpriteBatch, ssMessage.str().c_str(), Vector2(10, 10), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));


	wstringstream ssTimer;
	ssTimer << "Time left : " << timer << "s";
	mpFont->DrawString(mpSpriteBatch, ssTimer.str().c_str(), Vector2(10, 700), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

	wstringstream ssPickUps;
	ssPickUps << currentPickUps << "/" << pickupTotal << " pick ups!";
	mpFont->DrawString(mpSpriteBatch, ssPickUps.str().c_str(), Vector2(10, 50), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));

	mpSpriteBatch->End();
}

void UserInterface::Release(){
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
}