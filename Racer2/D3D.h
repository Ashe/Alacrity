#ifndef D3DH
#define D3DH

#include <d3d11.h>

#include "SimpleMath.h"

//main handle used to create resources and access D3D
extern ID3D11Device* gd3dDevice;
//a handle off the device we can use to give rendering commands
extern ID3D11DeviceContext* gd3dImmediateContext;
//a number of surfaces we can render onto and then display
extern IDXGISwapChain* gSwapChain;
//when we render things, where do the go?
extern ID3D11RenderTargetView* gRenderTargetView;
//when rendering we can test the depth of pixels, usually so we
//avoid rendering anything that is behind something else
extern ID3D11DepthStencilView* gDepthStencilView;
//position, height, width, min+max depth of the view we are rendering
extern D3D11_VIEWPORT gScreenViewport;
//a function to call when we ALT+ENTER or drag the window
//two parameters are width/height of the new window
extern void(*gpOnResize)(int, int);
//the data to render is just a big list of numbers, the gpu needs to know what they mean
//are they triangles, lines, coloured, textured, lit, not lit, ???
//using this information, the "input assembler" pulls the numbers out and presents them
//to the vertex shader
void InitInputAssembler(ID3D11InputLayout* pInputLayout, ID3D11Buffer* pVBuffer, UINT szVertex,
	ID3D11Buffer* pIBuffer, D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


//main start up function
bool InitDirect3D(void(*pOnResize)(int, int));
//default minimum behaviour when ALT+ENTER or drag or resize
//parameters are new width and height of window
void OnResize_Default(int clientWidth, int clientHeight);
//main shutdown function, don't forget to call it
//extraReporting gives a bit more information about any objects
//that we haven't released properly
void ReleaseD3D(bool extraReporting = true);
//is the screen/window square or letterbox or?
float GetAspectRatio();
//simplify the start+end of rendering
void BeginRender(const DirectX::SimpleMath::Vector4& backColour);
void EndRender();


#endif
