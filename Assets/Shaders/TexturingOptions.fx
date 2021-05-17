//====================================================================================================
// Filename:	Texturing.fx
// Created by:	Peter Chan
// Description:	Simple shader for texturing.
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}
cbuffer OptionsBuffer : register(b1)
{
    float4 translation;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, wvp);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float2 uvTranslate = input.texCoord;
    uvTranslate.r += translation.r;
    uvTranslate.g += translation.g;

    return diffuseMap.Sample(textureSampler, uvTranslate);
}