#ifndef PLAYER_H
#define PLAYER_H

#include "Mesh.h"
#include "Model.h"
#include "Input.h"
#include "LevelMGR.h"
#include "FX.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Player{
public:
	void Initialise(Mesh&, LevelMGR*, MouseAndKeys*);
	void Release();
	void Update(float);
	void Render(float);
	bool getMoveRequest();
	Vector3 getPlayerPostion();
	Vector2 getMoveDirection();

	//game models that reference meshes
	Model player;

	//handy rendering state
	FX::MyFX mFX;
private:
	int cellX;
	int cellY;
	bool moveRequest;
	Vector3 adjustVector;
	Vector2 moveDirection;
	LevelMGR* levelManager;

	MouseAndKeys* mMKInput;
};

#endif