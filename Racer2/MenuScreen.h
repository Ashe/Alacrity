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
	MenuScreen(FX::MyFX* fxRef, MouseAndKeys* mMKref, MeshManager* mMeshMGRref) : Screen(fxRef, mMKref, mMeshMGRref) {}
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
	bool isReset() const;
	void canReset();
	void setIsReset(const bool state);

private:
	void menuInputs();

	int screenW, screenH, screenCenterX, screenCenterY, menuOption, menuOptions;
	bool upPress, downPress, viewCredits, isResetB, canResetB;
	
	string mMessage;
	DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;
	DirectX::SpriteFont *mpFontTitle = nullptr;

	//camera
	float mAngle = 0;
	float mAngleSpeed = 1;
	DirectX::SimpleMath::Vector3 mCamPos;
	DirectX::SimpleMath::Vector2 camRadHeight = Vector2(45, 25);

	// Level Manager
	LevelMGR levelMGR = LevelMGR(mFX, mMeshMgr, DirectX::SimpleMath::Vector3(0, 0, 0));

	// Timer for animating menu
	Timer menuTimer;
};

#endif




