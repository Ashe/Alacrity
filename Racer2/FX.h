#ifndef FX_H
#define FX_H

#include <string>
#include <vector>
#include <d3d11.h>

#include "ShaderTypes.h"
#include "D3DUtil.h"

class Model;

namespace FX
{
	DirectX::SimpleMath::Matrix& GetProjectionMatrix();
	DirectX::SimpleMath::Matrix& GetViewMatrix();

	namespace RasterType { typedef enum { CCW_FILLED = 0, CCW_WIRE = 1, CW_FILLED = 2, CW_WIRE = 3, NOCULL_WIRE = 4, NOCULL_FILLED = 5, MAX_STATES = 6 }; }
	//when drawing primitives should they be filled/wireframe/clockwise culled/anti-clockwise culled/not culled at all
	void CreateRasterStates(ID3D11Device *pDevice, ID3D11RasterizerState *pStates[RasterType::MAX_STATES]);
	//load in and allocate a chunk of binary data
	char* ReadAndAllocate(const std::string& fileName, unsigned int& bytesRead);
	//we've loaded in a "blob" of compiled shader code, it needs to be set up on the gpu as a pixel shader
	void CreatePixelShader(char* pBuff, unsigned int buffSz, ID3D11PixelShader* &pPS);
	//we've loaded in a "blob" of compiled shader code, it needs to be set up on the gpu as a vertex shader
	void CreateVertexShader(char* pBuff, unsigned int buffSz, ID3D11VertexShader* &pVS);
	//the input assembler needs to know what is in the vertex buffer, how to get the data out and which vertex shader to give it to
	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC vdesc[], int numElements, char* pBuff, unsigned int buffSz, ID3D11InputLayout** pLayout);

	//if we want to give extra information to the shaders then it has to go in a constant buffer
	//once a shader runs, to render a leg or something, the data can't change until it's finished
	//so it's always a constant
	void CreateConstantBuffer(UINT sizeOfBuffer, ID3D11Buffer **pBuffer);
	//different hardware supports different instructions, lines of code, number of constants, etc
	//we cheat a bit and just go for shader model 5 which is really common and powerful
	void CheckShaderModel5Supported(ID3D11Device* pd3dDevice);
	//a sampler takes samples of the texture i.e. looks up texels
	void CreateSampler(ID3D11SamplerState* &pSampler);
	//there can be many blend states, this one uses the blend factors to directly control transparency
	void CreateTransparentBlendState(ID3D11Device* pd3dDevice, ID3D11BlendState* &pTransparent);
	//this one uses the texture alpha to control the transparency
	void CreateAlphaTransparentBlendState(ID3D11Device* pd3dDevice, ID3D11BlendState* &pBlend);


	//when passing data to the gpu it goes in constant buffers
	void CreateConstantBuffers();
	void ReleaseConstantBuffers();
	//set the constants that change per object
	void SetPerObjConsts(ID3D11DeviceContext *pD3DContext, DirectX::SimpleMath::Matrix &world);
	//set the constants that change each update
	void SetPerFrameConsts(ID3D11DeviceContext *pD3DContext, const DirectX::SimpleMath::Vector3& eyePos);
	/*create a directional light
	lightIdx - which light is it 0->7
	enable - turn it on or off
	direction - normal showing which way the light is shining
	diffuse, specular, ambient light intensities (0->1)
	*/
	void SetupDirectionalLight(int lightIdx, bool enable,
		const DirectX::SimpleMath::Vector3 &direction,
		const DirectX::SimpleMath::Vector3& diffuse = DirectX::SimpleMath::Vector3(1, 1, 1),
		const DirectX::SimpleMath::Vector3& ambient = DirectX::SimpleMath::Vector3(0, 0, 0),
		const DirectX::SimpleMath::Vector3& specular = DirectX::SimpleMath::Vector3(0, 0, 0));

	//create a point light
	//as above but specify a world position and don't bother with a direction
	//range - how far does the light shine
	//atten1 - really there are 3 attenuation values, but we'll just go with one
	//			it's enough to get reasonable control of how the light intensity falls away
	void SetupPointLight(int lightIdx, bool enable,
		const DirectX::SimpleMath::Vector3 &position,
		const DirectX::SimpleMath::Vector3& diffuse = DirectX::SimpleMath::Vector3(1, 1, 1),
		const DirectX::SimpleMath::Vector3& ambient = DirectX::SimpleMath::Vector3(0, 0, 0),
		const DirectX::SimpleMath::Vector3& specular = DirectX::SimpleMath::Vector3(0, 0, 0),
		float range = 1000.f, float atten1 = 0.05f);

	/*spot light
	As above but extra parameters to limit the angle of the inner/outer cone
	innerConeTheta - radius of inner bright cone
	outerConePhi - radius of outer cone as light falls off
	*/
	void SetupSpotLight(int lightIdx, bool enable,
		const DirectX::SimpleMath::Vector3 &position,
		const DirectX::SimpleMath::Vector3 &direction,
		const DirectX::SimpleMath::Vector3& diffuse = DirectX::SimpleMath::Vector3(1, 1, 1),
		const DirectX::SimpleMath::Vector3& ambient = DirectX::SimpleMath::Vector3(0, 0, 0),
		const DirectX::SimpleMath::Vector3& specular = DirectX::SimpleMath::Vector3(0, 0, 0),
		float range = 1000.f, float atten1 = 0.05f, float innerConeTheta = D2R(30), float outerConePhi = D2R(40));


	//like the mesh manager, we only ever want one unique texture to be loaded
	//it can then be shared between any meshes that need it
	class TexCache
	{
	public:
		//associate a file name with a texture resource
		struct Data
		{
			Data() {}
			Data(const std::string& fName, ID3D11ShaderResourceView*p, DirectX::SimpleMath::Vector2& _dim)
				: fileName(fName), pTex(p), dim(_dim) {}
			std::string fileName;
			ID3D11ShaderResourceView* pTex = nullptr;
			DirectX::SimpleMath::Vector2 dim;
		};

		//tidy up at the end
		void Release() {
			for (int i = 0; i < (int)mCache.size(); ++i)
				ReleaseCOM(mCache[i].pTex);
			mCache.clear();
		}
		//if this texture is new load it in, otherwise find it and return a handle
		ID3D11ShaderResourceView* LoadTexture(const std::string& fileName, bool appendPath, ID3D11Device*pDevice);
		//usually we just have a texture file name, but they're all in a sub folder
		void SetAssetPath(const std::string& path) {
			mAssetPath = path;
		}
		const std::string& GetAssetPath() const {
			return mAssetPath;
		}
		//pull out a texture by idx
		const Data& Get(int idx) {
			assert(idx < (int)mCache.size());
			return mCache[idx];
		}
		int GetIdx(const std::string& fileName);
	private:
		DirectX::SimpleMath::Vector2 GetDimensions(ID3D11ShaderResourceView* pTex);
		//array of texture data
		std::vector<Data> mCache;

		//some data sub folder with all the textures in
		std::string mAssetPath;
	};

	//bind common rendering data into a class, helps ensure 
	//initialization and shutdown events happen
	class MyFX
	{
	public:
		bool Init(ID3D11Device* pd3dDevice);
		void Release();
		//render	
		//model - one instance of a mesh with position/scale/rotation/etc
		//pD3DContext - handle to D3D
		//flags - control lighting, etc
		//pOverrideMat - if not null it points at a material to use instead of the one in the mesh
		void Render(Model& model, ID3D11DeviceContext *pD3DContext, MaterialExt* pOverrideMat = nullptr);
		//keep all the textures in one place then we know we only have each one once
		TexCache mCache;

	private:
		//called before rendering anything	
		void PreRenderObj(ID3D11DeviceContext *pD3DContext, MaterialExt& mat);
		//mapping between vertex/index buffers and gpu
		ID3D11InputLayout* mpInputLayout = nullptr;
		//a smapler to read the texture
		ID3D11SamplerState *mpSamAnisotropic = nullptr;
		//vertex and pixel shaders
		ID3D11VertexShader* mpVS = nullptr;
		//a complicated one if it's lit, a simple one if it isn't, also a textured option now (lit and unlit)
		ID3D11PixelShader* mpPSLit = nullptr, *mpPSUnlit = nullptr, *mpPSLitTex = nullptr, *mpPSUnlitTex = nullptr;
		//transparency means controlling the blend states beyond default settings
		ID3D11BlendState *mpBlendTransparent = nullptr, *mpBlendAlphaTrans = nullptr;
		//multiple raster states should be available
		ID3D11RasterizerState *mpRasterStates[RasterType::MAX_STATES];
		ID3D11DepthStencilState *mpDepthStencilDisable = nullptr;
	};

}
#endif
