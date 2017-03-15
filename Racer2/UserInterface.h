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
	void DisplayMessage(float dTime);
	void Release();
private:
	string mMessage;
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;
};

#endif