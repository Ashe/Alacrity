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
	UserInterface() {}
	~UserInterface() {}
	void Initialise();
	void DisplayMessage(string& message, float timer, int currentPickUps, int pickupTotal);
	void Release();
private:
	string mMessage;
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;
};

#endif