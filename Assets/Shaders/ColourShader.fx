//====================================================================================================
// Filename:	ColourShader.fx renamed from DoTransform.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix mvp
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, mvp);
	output.Color = Color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}