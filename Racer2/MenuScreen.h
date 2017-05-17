#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "Screen.h"

#include <vector>
#include <future>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"
#include "SpriteFont.h"
#include "Input.h"
#include "LevelMGR.h"
#include "Player.h"
#include "UserInterface.h"
#include "Timer.h"

//wrap up common behaviors, initialization+shutdown
class MenuScreen : public Screen
{
public:
	//start up and shut down
	MenuScreen(FX::MyFX* fxRef, MouseAndKeys* mMKref) : Screen(fxRef, mMKref) {}
	~MenuScreen() {
		Release();
	}
	//game logic, called constantly, elapsed time passed in
	int Update(float dTime);
	//render images, called constantly, elapsed time passed in
	void Render(float dTime);
	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	void Initialise();
	void Release();

private:
	
	string mMessage;
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;

	bool playerHoldingQuit = false;
};

#endif




