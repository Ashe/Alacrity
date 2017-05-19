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

void UserInterface::DisplayMessage(string& message, float timer, float maxTime, int currentPickUps, int pickupTotal){
	mpSpriteBatch->Begin();

	wstringstream ssMessage;
	ssMessage << "(Esc) Pause - (M) Mute" << endl << endl 
		<< message.c_str() << endl
		<< currentPickUps << "/" << pickupTotal << " pick ups!";
	mpFont->DrawString(mpSpriteBatch, ssMessage.str().c_str(), Vector2(10, 10), Colours::White, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));


	const float progress = timer / maxTime;
	float temp = (progress)* (screenWidth * 0.4);
	RECT barBorder = { screenCenterX - temp, screenHeight - 42.5, screenCenterX + temp, screenHeight - 22.5 };
	mpSpriteBatch->Draw(pmFX->mCache.LoadTexture("barp.dds", true, gd3dDevice), barBorder);

	temp = (progress)* (screenWidth * 0.4) - 2.5;
	RECT bar = { screenCenterX - temp, screenHeight - 40, screenCenterX + temp, screenHeight - 25 };

	Vector4 fullColour = Vector4{ 0, 1, 0.5f, 1 };
	Vector4 halfColour = Vector4{ 1, 1, 0, 0.5 };
	Vector4 lastColour = Vector4{ 1, 0, 0, 0 };

	Vector4 interp1, interp2;
	if (progress > halfColour.w) {
		interp1 = fullColour;
		interp2 = halfColour;
	}
	else {
		interp1 = halfColour;
		interp2 = lastColour;
	}

	Vector4 colour;
	const float interpProg = (progress - interp2.w) / (interp1.w - interp2.w);
	colour.x = interp2.x + (interpProg)* (interp1.x - interp2.x);
	colour.y = interp2.y + (interpProg)* (interp1.y - interp2.y);
	colour.z = interp2.z + (interpProg)* (interp1.z - interp2.z);
	colour.w = 1;

	mpSpriteBatch->Draw(pmFX->mCache.LoadTexture("barp.dds", true, gd3dDevice), bar, colour);



	mpSpriteBatch->End();
}

void UserInterface::setRes(int width, int height) {
	screenWidth = width;
	screenHeight = height;

	screenCenterX = width / 2;
	screenCenterY = height / 2;
}

void UserInterface::Release(){
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
}