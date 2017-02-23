#include <cassert>

#include "D3DUtil.h"
#include "D3D.h"
#include "WindowUtils.h"

using namespace std;

D3D_DRIVER_TYPE gd3dDriverType;
ID3D11Device* gd3dDevice;
ID3D11DeviceContext* gd3dImmediateContext;
UINT      g4xMsaaQuality;
IDXGISwapChain* gSwapChain;
ID3D11Texture2D* gDepthStencilBuffer;
ID3D11RenderTargetView* gRenderTargetView;
ID3D11DepthStencilView* gDepthStencilView;
D3D11_VIEWPORT gScreenViewport;
bool gEnable4xMsaa = true;
void(*gpOnResize)(int,int) = nullptr;
bool gWindowed = false;

float GetAspectRatio()
{
	int w, h;
	GetClientExtents(w, h);
	return static_cast<float>(w) / h;
}



// Resize the swap chain and recreate the render target view.
void ResizeSwapChain(IDXGISwapChain* pSwapChain, int screenWidth, int screenHeight, ID3D11Device* pd3dDevice, ID3D11RenderTargetView** pRenderTargetView)
{
	HR(pSwapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(pd3dDevice->CreateRenderTargetView(backBuffer, 0, pRenderTargetView));
	ReleaseCOM(backBuffer);
}

// Create the depth/stencil buffer description
void CreateDepthStencilDescription(D3D11_TEXTURE2D_DESC& dsd, int screenWidth, int screenHeight, bool msaa4X, int maxQuality)
{
	dsd.Width = screenWidth;
	dsd.Height = screenHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (msaa4X)
	{
		dsd.SampleDesc.Count = 4;
		dsd.SampleDesc.Quality = g4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
	}

	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;
}

// Create the depth/stencil buffer and view.
void CreateDepthStencilBufferAndView(D3D11_TEXTURE2D_DESC& dsd, ID3D11Texture2D** pDepthStencilBuffer, ID3D11DepthStencilView** pDepthStencilView)
{
	HR(gd3dDevice->CreateTexture2D(&dsd, 0, pDepthStencilBuffer));
	HR(gd3dDevice->CreateDepthStencilView(*pDepthStencilBuffer, 0, pDepthStencilView));
}

// Bind the render target view and depth/stencil view to the pipeline.
void BindRenderTargetViewAndDepthStencilView(ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pRenderTargetView, ID3D11DepthStencilView* pDepthStencilView)
{
	pd3dImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}

// Set the viewport transform.
void SetViewportDimensions(ID3D11DeviceContext* pd3dImmediateContext, int screenWidth, int screenHeight, D3D11_VIEWPORT& screenViewport)
{
	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.Width = static_cast<float>(screenWidth);
	screenViewport.Height = static_cast<float>(screenHeight);
	screenViewport.MinDepth = 0.0f;
	screenViewport.MaxDepth = 1.0f;

	pd3dImmediateContext->RSSetViewports(1, &screenViewport);
}


void CreateD3D(ID3D11Device** pd3dDevice, ID3D11DeviceContext** pd3dImmediateContext, D3D_DRIVER_TYPE& d3dDriverType, D3D_FEATURE_LEVEL desiredFeatureLevel = D3D_FEATURE_LEVEL_11_0)
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	gd3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

	D3D_FEATURE_LEVEL featureLevel;
	HR(D3D11CreateDevice(
		0,                 // default adapter
		d3dDriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		pd3dDevice,
		&featureLevel,
		pd3dImmediateContext));

	if (featureLevel != desiredFeatureLevel)
	{
		DBOUT("Direct3D Feature Level unsupported.");
		assert(false);
	}

}

void CheckMultiSamplingSupport(ID3D11Device* pd3dDevice, UINT& quality4xMsaa)
{
	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.
	// 4XMSAA looks good!
	HR(pd3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality4xMsaa));
	
	//if zero was returned then the hardware cannot do it
	assert(quality4xMsaa > 0);
}

void CreateSwapChainDescription(DXGI_SWAP_CHAIN_DESC& sd, HWND hMainWnd, bool windowed, int screenWidth, int screenHeight, bool msaa4X)
{
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (msaa4X)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = g4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hMainWnd;
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	gWindowed = windowed;
}

void CreateSwapChain(DXGI_SWAP_CHAIN_DESC& sd, ID3D11Device* pd3dDevice,IDXGISwapChain** pSwapChain)
{
	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	HR(gd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(pd3dDevice, &sd, pSwapChain));

	//now it's been created these factory objects are no longer needed
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
}


void OnResize_Default(int clientWidth, int clientHeight)
{
	assert(gd3dImmediateContext);
	assert(gd3dDevice);
	assert(gSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(gRenderTargetView);
	ReleaseCOM(gDepthStencilView);
	ReleaseCOM(gDepthStencilBuffer);


	ResizeSwapChain(gSwapChain, clientWidth, clientHeight, gd3dDevice, &gRenderTargetView);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	CreateDepthStencilDescription(depthStencilDesc, clientWidth, clientHeight, gEnable4xMsaa, g4xMsaaQuality - 1);
	CreateDepthStencilBufferAndView(depthStencilDesc, &gDepthStencilBuffer, &gDepthStencilView);

	BindRenderTargetViewAndDepthStencilView(gd3dImmediateContext, gRenderTargetView, gDepthStencilView);

	SetViewportDimensions(gd3dImmediateContext, clientWidth, clientHeight, gScreenViewport);

}


bool InitDirect3D(void(*pOnResize)(int,int))
{
	assert(pOnResize);
	gpOnResize = pOnResize;

	// Create the device and device context.
	CreateD3D(&gd3dDevice, &gd3dImmediateContext, gd3dDriverType);

	CheckMultiSamplingSupport(gd3dDevice, g4xMsaaQuality);

	int w, h;
	GetClientExtents(w, h);
	DXGI_SWAP_CHAIN_DESC sd;
	CreateSwapChainDescription(sd, GetMainWnd(), true, w, h, gEnable4xMsaa);
	CreateSwapChain(sd, gd3dDevice, &gSwapChain);


	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	gpOnResize(w, h);

	return true;
}

void ReleaseD3D(bool extraReporting)
{
	//check if full screen - not advisable to exit in full screen mode
	if (gSwapChain)
	{
		BOOL fullscreen = false;
		HR(gSwapChain->GetFullscreenState(&fullscreen, nullptr));
		if (fullscreen) //go for a window
			gSwapChain->SetFullscreenState(false, nullptr);
	}

	ReleaseCOM(gRenderTargetView);
	ReleaseCOM(gDepthStencilView);
	ReleaseCOM(gSwapChain);
	ReleaseCOM(gDepthStencilBuffer);

	// Restore all default settings.
	if (gd3dImmediateContext)
	{
		gd3dImmediateContext->ClearState();
		gd3dImmediateContext->Flush();
	}

	ReleaseCOM(gd3dImmediateContext);
	if (extraReporting)
	{
		ID3D11Debug* pd3dDebug;
		HR(gd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pd3dDebug)));
		HR(pd3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY));
		ReleaseCOM(pd3dDebug);
	}
	ReleaseCOM(gd3dDevice);
}

void InitInputAssembler(ID3D11InputLayout* pInputLayout, ID3D11Buffer* pVBuffer, UINT szVertex, ID3D11Buffer* pIBuffer, D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT offset = 0;
	gd3dImmediateContext->IASetVertexBuffers(0, 1, &pVBuffer , &szVertex, &offset);
	gd3dImmediateContext->IASetInputLayout(pInputLayout);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	gd3dImmediateContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	gd3dImmediateContext->IASetPrimitiveTopology(topology);
}

void EndRender()
{
	HR(gSwapChain->Present(0, 0));
}

void BeginRender(const DirectX::SimpleMath::Vector4& backColour)
{
	gd3dImmediateContext->ClearRenderTargetView(gRenderTargetView, reinterpret_cast<const float*>(&backColour));
	gd3dImmediateContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	gd3dImmediateContext->RSSetViewports(1, &gScreenViewport);
	gd3dImmediateContext->OMSetDepthStencilState(nullptr, 1);	//using sprites disables z-testing so reset to default (on)
}


