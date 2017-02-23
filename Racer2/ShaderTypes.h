#ifndef SHADERTYPES_H
#define SHADERTYPES_H

#include <d3d11.h>

#include "SimpleMath.h"

/*
This is what our vertex data will look like
*/
struct VertexPosNormTex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector3 Norm;
	DirectX::SimpleMath::Vector2 Tex;

	static const D3D11_INPUT_ELEMENT_DESC sVertexDesc[3];
};

/*
Insted of a colour in each vertex we define a material
for a group of primitves (an entire surface)
*/
struct Material
{
	Material(const DirectX::SimpleMath::Vector4& d, const DirectX::SimpleMath::Vector4& a, const DirectX::SimpleMath::Vector4& s) {
		Diffuse = d;
		Ambient = a;
		Specular = s;
	}
	Material() : Diffuse(1, 1, 1, 1), Ambient(1, 1, 1, 1),Specular(0, 0, 0, 1) {}
	void Set(const DirectX::SimpleMath::Vector4&d, const DirectX::SimpleMath::Vector4& a, const DirectX::SimpleMath::Vector4& s) {
		Diffuse = d; Ambient = a; Specular = s;
	}

	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Specular; // w = SpecPower
};

//one material per mesh
struct MaterialExt
{
	struct TexTrsfm
	{
		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1, 1);
		float angle = 0;
		DirectX::SimpleMath::Vector2 translate = DirectX::SimpleMath::Vector2(0, 0);
	};

	MaterialExt() : flags(TFlags::APPEND_PATH | TFlags::LIT | TFlags::CULL | TFlags::CCW_WINDING | TFlags::ZTEST) {
		blendFactors[0] = blendFactors[1] = blendFactors[2] = blendFactors[3] = 1;
		pTextureRV = nullptr;
	}
	MaterialExt(const Material &mat, ID3D11ShaderResourceView *pTex, TexTrsfm texTrsfm, int _flags, const std::string& _name, const std::string& file)
		: gfxData(mat), name(_name), texture(file), flags(_flags) {
		blendFactors[0] = blendFactors[1] = blendFactors[2] = blendFactors[3] = 1;
		pTextureRV = pTex;
	}
	void SetBlendFactors(float r, float g, float b, float a) {
		blendFactors[0] = r; blendFactors[1] = g; blendFactors[2] = b; blendFactors[3] = a;
	}

	Material gfxData;	//this is the original material reflection data that gets passed to the shader

	ID3D11ShaderResourceView *pTextureRV;	//handled by effects texture cache so don't release

	TexTrsfm texTrsfm;


	typedef enum {
	 	APPEND_PATH = 1,	//if true then the texture file name is just a name and needs a path pre-appending before use
		TRANSPARENCY = 2,		//use blend factors to force transparency
		LIT = 4,				//reflects light
		ALPHA_TRANSPARENCY = 8,	//use the texture alpha
		CULL = 16,			//remove back facing primitives
		CCW_WINDING = 32,	//counter clockwise winding or clockwise - defines which side is front (for culling)
		WIRE_FRAME = 64,		//solid or wireframe?
		ZTEST = 128
	} TFlags;
	int flags;
	float blendFactors[4];
	std::string name;			//material names can come from 3DSMax and can be useful for debugging
	std::string texture;		//file name of texture

	static const MaterialExt default;
};


struct Light
{
	Light() { type = OFF; }

	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;		//w=nothing
	DirectX::SimpleMath::Vector4 Direction;		//w=nothing
	DirectX::SimpleMath::Vector4 Position;		//w=nothing
	DirectX::SimpleMath::Vector4 Attenuation;	//w=nothing

	typedef enum {
		OFF = 0,		//ignore this light
		DIR = 1,	//directional
		POINT = 2,	//point light
		SPOT = 3	//spot light
	} Type;
	int type;
	float range;
	float theta;
	float phi;
};

//shader variables that don't change within one frame
const int MAX_LIGHTS = 8;
struct GfxParamsPerFrame
{
	Light lights[MAX_LIGHTS];
	DirectX::SimpleMath::Vector4 eyePosW;
};
static_assert((sizeof(GfxParamsPerFrame) % 16) == 0, "CB size not padded correctly");



//shader variables that don't change within one object
struct GfxParamsPerObj
{
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix worldInvT;
	DirectX::SimpleMath::Matrix worldViewProj;
	DirectX::SimpleMath::Matrix view;
};
static_assert((sizeof(GfxParamsPerObj) % 16) == 0, "CB size not padded correctly");


//shader variables that don't change within an object's sub-mesh
struct GfxParamsPerMesh
{
	DirectX::SimpleMath::Matrix textureTrsfm;
	Material material;
};
static_assert((sizeof(GfxParamsPerMesh) % 16) == 0, "CB size not padded correctly");


#endif
