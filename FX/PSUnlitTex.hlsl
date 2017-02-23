#include "Constants.hlsl"

float4 main(VertexOut pin) : SV_Target
{
	//sum intensities 
	float4 litColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a * litColor.a;

	return litColor;
}