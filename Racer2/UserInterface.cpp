#include <iomanip>
#include <sstream>

#include "WindowUtils.h" 
#include "D3D.h"
#include "UserInterface.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void UserInterface::Initialise(){
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);
	mpFont = new SpriteFont(gd3dDevice, L"data/comicSansMS.spritefont");
	assert(mpFont);
}

void UserInterface::DisplayMessage(float dTime){
	mpSpriteBatch->Begin();

	wstringstream ss;
	ss << "HELLO!";
	mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(10, 550), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));

	mpSpriteBatch->End();
}

void UserInterface::Release(){
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
}