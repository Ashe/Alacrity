#ifndef SCREEN_H
#define SCREEN_H

#include "FX.h"
#include "Input.h"
#include "Mesh.h"

class Screen abstract {
public:
	Screen(FX::MyFX* fxRef, MouseAndKeys* mMKref, MeshManager* mMeshMGRref) : mFX(fxRef), mMKInput(mMKref), mMeshMgr(mMeshMGRref) {}
	virtual void Initialise() = 0;
	virtual int Update(float dTime) = 0;
	virtual void Render(float dTime) = 0;
	virtual void OnResize(int screenWidth, int screenHeight) = 0;
	virtual void Release() = 0;

	FX::MyFX*mFX;
	MouseAndKeys* mMKInput;
	MeshManager* mMeshMgr;

private:
};
#endif