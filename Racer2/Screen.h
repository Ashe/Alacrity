#ifndef SCREEN_H
#define SCREEN_H

#include "FX.h"

class Screen abstract {
public:
	Screen(FX::MyFX* fxRef) : mFX(fxRef) {}
	virtual void Initialise() = 0;
	virtual void Update(float dTime) = 0;
	virtual void Render(float dTime) = 0;
	virtual void OnResize(int screenWidth, int screenHeight) = 0;
	virtual void Release() = 0;
	virtual LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	FX::MyFX*mFX;
private:
};
#endif