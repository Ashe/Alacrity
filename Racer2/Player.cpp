#include "WindowUtils.h"
#include "D3D.h"
#include "Player.h"
#include "GeometryBuilder.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Player::Initialise(Mesh& playerMesh, LevelMGR* levelPointer, MouseAndKeys* mMKPointer){

	mFX.Init(gd3dDevice);

	player.Initialise(playerMesh);
	player.GetPosition() = Vector3(0, 0, 2);
	adjustVector = { 0, 0, 2 };
	levelManager = levelPointer;
	mMKInput = mMKPointer;
}
void Player::Release(){

}
void Player::Update(float dTime){
	bool success = false;

	if (mMKInput->IsPressed(VK_W)){
		moveDirection = Vector2(0, 1);
		adjustVector = levelManager->move(player.GetPosition(), moveDirection, success);
	}else if (mMKInput->IsPressed(VK_A)){
		moveDirection = Vector2(-1, 0);
		adjustVector = levelManager->move(player.GetPosition(), moveDirection, success);
	}else if (mMKInput->IsPressed(VK_S)){
		moveDirection = Vector2(0, -1);
		adjustVector = levelManager->move(player.GetPosition(), moveDirection, success);
	}else if (mMKInput->IsPressed(VK_D)){
		moveDirection = Vector2(1, 0);
		adjustVector = levelManager->move(player.GetPosition(), moveDirection, success);
	}
	if (!success)
		moveDirection = Vector2(0, 0);
}
void Player::Render(float dTime){
	mFX.Render(player, gd3dImmediateContext);

	player.GetPosition() = adjustVector;
}
bool Player::getMoveRequest(){
	bool state = moveRequest;
	moveRequest = false;
	return state;
}
Vector3 Player::getPlayerPostion(){
	return player.GetPosition();
}
Vector2 Player::getMoveDirection(){
	return moveDirection;
}