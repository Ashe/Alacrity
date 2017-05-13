#include "WindowUtils.h"
#include "D3D.h"
#include "Player.h"
#include "GeometryBuilder.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Player::Initialise(FX::MyFX* fxRef, Mesh& playerMesh, LevelMGR* levelPointer, MouseAndKeys* mMKPointer){
	mFX = fxRef;
	player.Initialise(playerMesh);
	player.GetPosition() = levelPointer->getStartingPosition();
	adjustVector = player.GetPosition();
	levelManager = levelPointer;
	mMKInput = mMKPointer;
	buttonHold = false;
	moveWait = 0;
	playerOne = true;
}
void Player::Release(){

}
void Player::Update(float dTime){
	bool success = false;

	move();

	if ((!buttonHold || (buttonHold && moveWait <= 0)) && (moveDirection.x != 0 || moveDirection.y != 0)){
		adjustVector = levelManager->move(player.GetPosition(), moveDirection, success);
		moveWait = 250;
	}
	else // If the player is not moving, retrieve the current tile's coords
		adjustVector = levelManager->getCurrentLocationOfTile(adjustVector);

	if (moveDirection != Vector2(0, 0))
		buttonHold = true;
	else
		buttonHold = false;

	moveWait -= dTime;

	moveDirection = Vector2(0, 0);
}
void Player::Render(float dTime){
	mFX->Render(player, gd3dImmediateContext);
	player.GetPosition() = adjustVector;

	// This is so that the player sits on top of his tile. You need to add the size of the player on the end so that he's not inside it
	player.GetPosition().z += 2;
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
void Player::makePlayerOne(){
	playerOne = true;
}
void Player::makePlayerTwo(){
	playerOne = false;
}
void Player::move(){
	if (playerOne){
		if (mMKInput->IsPressed(VK_W)){
			moveDirection = Vector2(0, 1);
		}
		else if (mMKInput->IsPressed(VK_A)){
			moveDirection = Vector2(-1, 0);
		}
		else if (mMKInput->IsPressed(VK_S)){
			moveDirection = Vector2(0, -1);
		}
		else if (mMKInput->IsPressed(VK_D)){
			moveDirection = Vector2(1, 0);
		}
	}else{
		if (mMKInput->IsPressed(VK_UP)){
			moveDirection = Vector2(0, 1);
		}
		else if (mMKInput->IsPressed(VK_LEFT)){
			moveDirection = Vector2(-1, 0);
		}
		else if (mMKInput->IsPressed(VK_DOWN)){
			moveDirection = Vector2(0, -1);
		}
		else if (mMKInput->IsPressed(VK_RIGHT)){
			moveDirection = Vector2(1, 0);
		}
	}
}