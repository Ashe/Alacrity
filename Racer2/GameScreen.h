#ifndef GAMESCREEN_H
#define GAMESCREEN_H

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
class GameScreen : public Screen
{
public:
	//start up and shut down
	GameScreen(FX::MyFX* fxRef, MouseAndKeys* mMKref, MeshManager* mMeshMGRref) : Screen(fxRef, mMKref, mMeshMGRref) {}
	~GameScreen() {
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


	//camera
	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 0.5f, -5);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(5, 4, -6);

	// Level Manager
	LevelMGR levelMGR = LevelMGR(mFX, mMeshMgr, DirectX::SimpleMath::Vector3(0, 0, 0));

	Model mQuad;
private:
	Player player;

	//load meshes
	void Load();
	//display a message
	void LoadDisplay(float dTime);
	//light position
	DirectX::SimpleMath::Vector3 mLightPos = DirectX::SimpleMath::Vector3(0, 0, 0);
	

	UserInterface mUI;
};

#endif




