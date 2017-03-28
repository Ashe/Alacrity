#ifndef GAME_H
#define GAME_H

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
class Game
{
public:
	//start up and shut down
	Game() {}
	~Game() {
		Release();
	}
	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);
	//render images, called constantly, elapsed time passed in
	void Render(float dTime);
	//called when ALT+ENTER or drag
	void OnResize(int screenWidth, int screenHeight);
	void Initialise();
	void Release();
	//message handling
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	
	//camera
	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 0.5f, -5);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(5, 4, -6);

	//ensure each mesh is loaded and release exactly once
	MeshManager mMeshMgr;

	// Level Manager
	LevelMGR levelMGR = LevelMGR(mFX, mMeshMgr, DirectX::SimpleMath::Vector3(0, 0, 0));

	//handy rendering state
	FX::MyFX mFX;


	Model mQuad;
private:
	Game& operator=(const Game&) = delete;
	Game(const Game& m) = delete;

	Player player;

	//load meshes
	void Load();
	//display a message
	void LoadDisplay(float dTime);
	//light position
	DirectX::SimpleMath::Vector3 mLightPos = DirectX::SimpleMath::Vector3(0, 0, 0);
	//text
	//DirectX::SpriteBatch *mpSpriteBatch = nullptr;
	//DirectX::SpriteFont *mpFont = nullptr, *mpFont2 = nullptr;
	
	//loading handler
	struct LoadData
	{
		//second thread
		std::future<void> loader;
		//how many meshes to load
		int totalToLoad = 0;
		//tally
		int loadedSoFar = 0;
		//is the loader active
		bool running = false;
	};
	LoadData mLoadData;
	MouseAndKeys mMKInput;
	UserInterface mUI;
};

#endif




