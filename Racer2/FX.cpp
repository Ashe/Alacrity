#include <fstream>
#include <assert.h>

#include "DDSTextureLoader.h"

#include "D3D.h"
#include "D3DUtil.h"
#include "FX.h"
#include "Model.h"
#include "Mesh.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace FX
{


GfxParamsPerObj gGfxPerObj;
GfxParamsPerFrame gGfxPerFrame;
GfxParamsPerMesh gGfxPerMesh;
ID3D11Buffer *gpGfxPerObj = nullptr, *gpGfxPerFrame = nullptr, *gpGfxPerMesh = nullptr;
Matrix gView, gProj;

DirectX::SimpleMath::Matrix& GetProjectionMatrix()
{
	return gProj;
}

DirectX::SimpleMath::Matrix& GetViewMatrix()
{
	return gView;
}


void CreateConstantBuffers()
{
	CreateConstantBuffer(sizeof(GfxParamsPerFrame), &gpGfxPerFrame);
	CreateConstantBuffer(sizeof(GfxParamsPerObj), &gpGfxPerObj);
	CreateConstantBuffer(sizeof(GfxParamsPerMesh), &gpGfxPerMesh);
}

void ReleaseConstantBuffers()
{
	ReleaseCOM(gpGfxPerFrame);
	ReleaseCOM(gpGfxPerObj);
	ReleaseCOM(gpGfxPerMesh);
}

void SetPerObjConsts(ID3D11DeviceContext *pD3DContext, DirectX::SimpleMath::Matrix& world)
{
	gGfxPerObj.world = world;
	gGfxPerObj.worldInvT = InverseTranspose(world);
	gGfxPerObj.worldViewProj = world * gView * gProj;
	gGfxPerObj.view = gView;
	pD3DContext->UpdateSubresource(gpGfxPerObj, 0, nullptr, &gGfxPerObj, 0, 0);
}

void SetPerFrameConsts(ID3D11DeviceContext *pD3DContext, const Vector3& eyePos)
{
	gGfxPerFrame.eyePosW = Vector4(eyePos.x, eyePos.y, eyePos.z, 0);
	pD3DContext->UpdateSubresource(gpGfxPerFrame, 0, nullptr, &gGfxPerFrame, 0, 0);

}

void CreateRasterStates(ID3D11Device *pDevice, ID3D11RasterizerState *pStates[RasterType::MAX_STATES])
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FrontCounterClockwise = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::CCW_FILLED]));

	desc.FillMode = D3D11_FILL_WIREFRAME;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::CCW_WIRE]));

	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::CW_FILLED]));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::CW_WIRE]));

	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::NOCULL_WIRE]));
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	HR(pDevice->CreateRasterizerState(&desc, &pStates[RasterType::NOCULL_FILLED]));
}


void CreateTransparentBlendState(ID3D11Device* pd3dDevice, ID3D11BlendState* &pTransparent)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;

	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	HR(pd3dDevice->CreateBlendState(&blendDesc, &pTransparent));


}


void CreateAlphaTransparentBlendState(ID3D11Device* pd3dDevice, ID3D11BlendState* &pBlend)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;	
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;

	rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtbd;
	//rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;

	HR(pd3dDevice->CreateBlendState(&blendDesc, &pBlend));
}

void CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState* &pResult)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.DepthEnable = enable;
	desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = false;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	desc.BackFace = desc.FrontFace;

	HR(gd3dDevice->CreateDepthStencilState(&desc, &pResult));
}

void SetupDirectionalLight(int lightIdx, bool enable, const Vector3 &direction,
	const Vector3& diffuse, const Vector3& ambient, const Vector3& specular)
{
	assert(lightIdx >= 0 && lightIdx < 8);
	if (!enable)
	{
		gGfxPerFrame.lights[lightIdx].type = Light::Type::OFF;
		return;
	}

	Light& l = gGfxPerFrame.lights[lightIdx];
	l.type = Light::Type::DIR;
	l.Diffuse = Vec3To4(diffuse, 0);
	l.Ambient = Vec3To4(ambient, 0);
	l.Specular = Vec3To4(specular,0);
	l.Direction = Vec3To4(direction, 0);
}

void SetupPointLight(int lightIdx, bool enable,
	const Vector3& position,
	const Vector3& diffuse,
	const Vector3& ambient,
	const Vector3& specular,
	float range/*=1000.f*/, float atten1/*=0.05f*/)
{
	assert(lightIdx >= 0 && lightIdx < 8);
	if (!enable)
	{
		gGfxPerFrame.lights[lightIdx].type = Light::Type::OFF;
		return;
	}

	Light& l = gGfxPerFrame.lights[lightIdx];
	l.type = Light::Type::POINT;
	l.Diffuse = Vec3To4(diffuse, 0);
	l.Ambient = Vec3To4(ambient, 0);
	l.Specular = Vec3To4(specular,0);
	l.Position = Vec3To4(position, 0);
	l.Attenuation = Vector4(0, atten1, 0, 0);
	l.range = range;
}

void SetupSpotLight(int lightIdx, bool enable,
	const DirectX::SimpleMath::Vector3 &position,
	const DirectX::SimpleMath::Vector3 &direction,
	const DirectX::SimpleMath::Vector3& diffuse,
	const DirectX::SimpleMath::Vector3& ambient,
	const DirectX::SimpleMath::Vector3& specular,
	float range, float atten1, float innerConeTheta, float outerConePhi)
{
	assert(lightIdx >= 0 && lightIdx < 8);
	if (!enable)
	{
		gGfxPerFrame.lights[lightIdx].type = Light::Type::OFF;
		return;
	}

	Light& l = gGfxPerFrame.lights[lightIdx];
	l.type = Light::Type::SPOT;
	l.Diffuse = Vec3To4(diffuse, 0);
	l.Ambient = Vec3To4(ambient, 0);
	l.Specular = Vec3To4(specular,0);
	l.Position = Vec3To4(position, 0);
	l.Direction = Vec3To4(direction, 0);
	l.Attenuation = Vector4(0, atten1, 0, 0);
	l.range = range;
	l.theta = innerConeTheta;
	l.phi = outerConePhi;
}


void CheckShaderModel5Supported(ID3D11Device* pd3dDevice)
{
	D3D_FEATURE_LEVEL featureLevel = pd3dDevice->GetFeatureLevel();
	bool featureLevelOK = true;
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
		break;
	default:
		featureLevelOK = false;
	}

	if (!featureLevelOK)
	{
		DBOUT("feature level too low for shader model 5");
		assert(false);
	}
}

void CreateConstantBuffer(UINT sizeOfBuffer, ID3D11Buffer **pBuffer)
{
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeOfBuffer;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(gd3dDevice->CreateBuffer(&constantBufferDesc, nullptr, pBuffer));

}

//used to load a file as a chunk of binary, the buffer returned needs to be deleted
char* ReadAndAllocate(const string& fileName, unsigned int& bytesRead)
{
	//open the file for reading
	ifstream infile;
	infile.open(fileName, ios::binary | ios::in);
	if (!infile.is_open() || infile.fail())
	{
		DBOUT("failed to open file: " << fileName);
		assert(false);
		return nullptr;
	}

	//read the whole contents
	infile.seekg(0, ios::end);
	ifstream::streampos size = infile.tellg();
	if (size > INT_MAX || size <= 0)
	{
		DBOUT("failed to get size of file: " << fileName);
		assert(false);
	}
	char* pBuff = new char[(int)size];
	infile.seekg(0, ios::beg);
	infile.read(pBuff, size);
	if (infile.fail())
	{
		DBOUT("failed to read file: " << fileName);
		assert(false);
	}
	infile.close();
	bytesRead = (int)size;
	return pBuff;
}

void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC vdesc[], int numElements, char* pBuff, unsigned int buffSz, ID3D11InputLayout** pLayout)
{
	assert(pBuff);
	HR(gd3dDevice->CreateInputLayout(vdesc, numElements, pBuff, buffSz, pLayout));

}

void CreateVertexShader(char* pBuff, unsigned int buffSz, ID3D11VertexShader* &pVS)
{
	assert(pBuff);
	HR(gd3dDevice->CreateVertexShader(pBuff,
		buffSz,
		nullptr,
		&pVS));
	assert(pVS);
}

void CreatePixelShader(char* pBuff, unsigned int buffSz, ID3D11PixelShader* &pPS)
{
	assert(pBuff);
	HR(gd3dDevice->CreatePixelShader(pBuff,
		buffSz,
		nullptr,
		&pPS));
	assert(pPS);
}

void CreateSampler(ID3D11SamplerState* &pSampler)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(gd3dDevice->CreateSamplerState(&sampDesc, &pSampler));
}


bool MyFX::Init(ID3D11Device* pd3dDevice)
{
	CheckShaderModel5Supported(pd3dDevice);
	CreateSampler(mpSamAnisotropic);
	CreateRasterStates(pd3dDevice, mpRasterStates);

	char* pBuff = nullptr;
	unsigned int bytes = 0;
	pBuff = ReadAndAllocate("data/TextureVS.cso", bytes);
	CreateVertexShader(pBuff, bytes, mpVS);
	CreateInputLayout(VertexPosNormTex::sVertexDesc, 3, pBuff, bytes, &mpInputLayout);
	delete[] pBuff;

	pBuff = ReadAndAllocate("data/PSLitNoTex.cso", bytes);
	CreatePixelShader(pBuff, bytes, mpPSLit);
	delete[] pBuff;

	pBuff = ReadAndAllocate("data/PSUnlitNoTex.cso", bytes);
	CreatePixelShader(pBuff, bytes, mpPSUnlit);
	delete[] pBuff;

	pBuff = ReadAndAllocate("data/PSLitTex.cso", bytes);
	CreatePixelShader(pBuff, bytes, mpPSLitTex);
	delete[] pBuff;

	pBuff = ReadAndAllocate("data/PSUnlitTex.cso", bytes);
	CreatePixelShader(pBuff, bytes, mpPSUnlitTex);
	delete[] pBuff;

	CreateConstantBuffers();
	CreateTransparentBlendState(pd3dDevice, mpBlendTransparent);
	CreateAlphaTransparentBlendState(pd3dDevice, mpBlendAlphaTrans);
	CreateDepthStencilState(false, false, mpDepthStencilDisable);

	//preload textures we'll need
	mCache.SetAssetPath("data/");

	return true;
}

void MyFX::Release()
{
	ReleaseCOM(mpVS);
	ReleaseCOM(mpPSLit);
	ReleaseCOM(mpPSUnlit);
	ReleaseCOM(mpPSLitTex);
	ReleaseCOM(mpPSUnlitTex);
	ReleaseCOM(mpInputLayout);
	ReleaseCOM(mpSamAnisotropic);
	ReleaseCOM(mpBlendTransparent);
	ReleaseCOM(mpBlendAlphaTrans);
	ReleaseCOM(mpDepthStencilDisable);
	ReleaseConstantBuffers();
	for (int i = 0; i < RasterType::MAX_STATES; ++i)
		ReleaseCOM(mpRasterStates[i]);
	mCache.Release();
}

void MyFX::Render(Model& model, ID3D11DeviceContext *pD3DContext, MaterialExt* pOverrideMat)
{
	//setup shaders
	pD3DContext->VSSetShader(mpVS, nullptr, 0);

	Matrix w;
	model.GetWorldMatrix(w);
	SetPerObjConsts(pD3DContext, w);

	Mesh& mesh = model.GetMesh();
	for (int i = 0; i < mesh.GetNumSubMeshes(); ++i)
	{
		//update material
		SubMesh& sm = mesh.GetSubMesh(i);

		InitInputAssembler(mpInputLayout, sm.mpVB, sizeof(VertexPosNormTex), sm.mpIB);
		MaterialExt *pM;
		if (pOverrideMat)
			pM = pOverrideMat;
		else if (model.HasOverrideMat())
			pM = model.HasOverrideMat();
		else
			pM = &sm.material;


		PreRenderObj(pD3DContext, *pM);
		pD3DContext->DrawIndexed(sm.mNumIndices, 0, 0);
	
	}
	pD3DContext->OMSetBlendState(0, 0, 0xffffffff);
}

void MyFX::PreRenderObj(ID3D11DeviceContext *pD3DContext, MaterialExt& mat)
{
	//texture transform
	gGfxPerMesh.material = mat.gfxData;
	gGfxPerMesh.textureTrsfm = Matrix::CreateScale(mat.texTrsfm.scale.x, mat.texTrsfm.scale.y, 1) *
		Matrix::CreateRotationZ(mat.texTrsfm.angle) *
		Matrix::CreateTranslation(mat.texTrsfm.translate.x, mat.texTrsfm.translate.y, 0);
	pD3DContext->UpdateSubresource(gpGfxPerMesh, 0, nullptr, &gGfxPerMesh, 0, 0);

	//buffers
	pD3DContext->VSSetConstantBuffers(0, 1, &gpGfxPerFrame);
	pD3DContext->VSSetConstantBuffers(1, 1, &gpGfxPerObj);
	pD3DContext->VSSetConstantBuffers(2, 1, &gpGfxPerMesh);

	pD3DContext->PSSetConstantBuffers(0, 1, &gpGfxPerFrame);
	pD3DContext->PSSetConstantBuffers(1, 1, &gpGfxPerObj);
	pD3DContext->PSSetConstantBuffers(2, 1, &gpGfxPerMesh);

	//select pixel shader to use
	ID3D11PixelShader* p;
	if ((mat.flags&MaterialExt::TFlags::LIT)!=0)
	{
		if (mat.pTextureRV)
			p = mpPSLitTex;
		else
			p = mpPSLit;
	}
	else
	{
		if (mat.pTextureRV)
			p = mpPSUnlitTex;
		else
			p = mpPSUnlit;
	}
	//do we have a texture
	if (mat.pTextureRV)
	{
		pD3DContext->PSSetSamplers(0, 1, &mpSamAnisotropic);
		pD3DContext->PSSetShaderResources(0, 1, &mat.pTextureRV);
	}
	pD3DContext->PSSetShader(p, nullptr, 0);

	//how is it blended?
	if ((mat.flags&MaterialExt::TFlags::TRANSPARENCY) != 0)
		pD3DContext->OMSetBlendState(mpBlendTransparent, mat.blendFactors, 0xffffffff);
	else if ((mat.flags&MaterialExt::TFlags::ALPHA_TRANSPARENCY) != 0)
	{
		float b[] = { 1, 1, 1, 1 };
		pD3DContext->OMSetBlendState(mpBlendAlphaTrans, b, 0xffffffff);
	}

	//should we cull?
	if ((mat.flags&MaterialExt::TFlags::CULL) == 0)
		if ((mat.flags&MaterialExt::TFlags::WIRE_FRAME) != 0)
			pD3DContext->RSSetState(mpRasterStates[RasterType::NOCULL_WIRE]);
		else
			pD3DContext->RSSetState(mpRasterStates[RasterType::NOCULL_FILLED]);
	else if ((mat.flags&MaterialExt::TFlags::CCW_WINDING) != 0)
		if ((mat.flags&MaterialExt::TFlags::WIRE_FRAME) != 0)
			pD3DContext->RSSetState(mpRasterStates[RasterType::CCW_WIRE]);
		else
			pD3DContext->RSSetState(mpRasterStates[RasterType::CCW_FILLED]);
	else
		if ((mat.flags&MaterialExt::TFlags::WIRE_FRAME) != 0)
			pD3DContext->RSSetState(mpRasterStates[RasterType::CW_WIRE]);
		else
			pD3DContext->RSSetState(mpRasterStates[RasterType::CW_FILLED]);

	if ((mat.flags&MaterialExt::TFlags::ZTEST) != 0)
		pD3DContext->OMSetDepthStencilState(nullptr, 1);
	else
		pD3DContext->OMSetDepthStencilState(mpDepthStencilDisable, 0);

}


ID3D11ShaderResourceView* TexCache::LoadTexture(const std::string& fileName, bool appendPath, ID3D11Device*pDevice)
{
	//search the cache
	Data *pSlot = nullptr;
	for (int i = 0; i < (int)mCache.size(); ++i)
		if (fileName == mCache[i].fileName)
			//we already have the texture so just return it
			return mCache[i].pTex;

	
	//prepare the path for loading
	const string *pPath = &fileName;
	string path;
	if (appendPath)
	{
		path = mAssetPath + fileName;
		pPath = &path;
	}
	std::wstring ws(pPath->begin(), pPath->end());
	//load it
	DDS_ALPHA_MODE alpha;
	ID3D11ShaderResourceView *pT;
	if (CreateDDSTextureFromFile(pDevice, ws.c_str(), nullptr, &pT, 0, &alpha) != S_OK)
	{
		DBOUT("Cannot load " << *pPath << "\n");
		assert(false);
	}
	//save it
	mCache.push_back(Data(fileName, pT, GetDimensions(pT)));
	return mCache.back().pTex;
}

Vector2 TexCache::GetDimensions(ID3D11ShaderResourceView* pTex)
{
	assert(pTex);
	ID3D11Resource* res = nullptr;
	pTex->GetResource(&res);
	assert(res);
	ID3D11Texture2D* texture2d = nullptr;
	HRESULT hr = res->QueryInterface(&texture2d);
	Vector2 dim(0, 0);
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2d->GetDesc(&desc);
		dim.x = static_cast<float>(desc.Width);
		dim.y = static_cast<float>(desc.Height);
	}
	ReleaseCOM(texture2d);
	ReleaseCOM(res);
	return dim;
}

int TexCache::GetIdx(const std::string& fileName)
{
	for (int i = 0; i < (int)mCache.size(); ++i)
		if (fileName == mCache[i].fileName)
			return i;
	DBOUT("Bad texture name" << fileName);
	assert(false);
	return -1;
}

}