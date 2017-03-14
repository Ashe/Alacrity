#include <thread>
#include <iomanip>

#include "WindowUtils.h" 
#include "D3D.h"
#include "LevelMGR.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void LevelMGR::OnResize(int screenWidth, int screenHeight)
{

	OnResize_Default(screenWidth, screenHeight);
}

void LevelMGR::Initialise()
{
	changeLevel(0);
}

void LevelMGR::Release()
{
	level.Release();
}

void LevelMGR::Update(float dTime)
{
	level.Update(dTime);
}


void LevelMGR::Render(float dTime)
{
	level.Render(dTime);
}

void LevelMGR::changeLevel(int levelNo) {
	levelTemplate nextLevel = getLayout(levelNo);
	level.Initialise(nextLevel.tileList, nextLevel.tilePadding, nextLevel.tileDim);
}

LevelMGR::levelTemplate LevelMGR::getLayout(int layoutNo) {

	levelTemplate nextLevel;

	switch (layoutNo)
	{
		case 0:
		{
			nextLevel.tileDim = 10;
			nextLevel.tilePadding = 1.25;

			nextLevel.tileList = {

				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic}

			};

			break;
		}
	}

	return nextLevel;
}

int LevelMGR::getLayoutDim(int levelNo) {
	return 0;
}