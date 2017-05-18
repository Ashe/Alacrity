#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <vector>
#include <future>

#include "FX.h"
#include "SpriteFont.h"
#include "Input.h"

using namespace std;

class UserInterface
{
public:
	UserInterface(FX::MyFX* mFX) : pmFX(mFX) {}
	~UserInterface() {}
	void Initialise();
	void DisplayMessage(string& message, float timer, float maxTimer, int currentPickUps, int pickupTotal);
	void Release();

	void setRes(int, int);
private:
	string mMessage;
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;

	FX::MyFX* pmFX;

	int screenWidth;
	int screenHeight;

	float screenCenterX;
	float screenCenterY;
};

#endif