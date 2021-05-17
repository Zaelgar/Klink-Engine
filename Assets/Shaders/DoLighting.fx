//====================================================================================================
// Filename:	Texturing.fx
// Created by:	Peter Chan
// Description:	Simple shader for texturing.
//====================================================================================================

cbuffer TransformBuffer : register(b0)
{
    matrix world;
	matrix wvp;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 materialPower;
}

struct VSInput
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
};

struct VSOutput
{
	float4 position	: SV_POSITION;
	float4 color : COLOR;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position, wvp);

	// Regardless of lights, the ambient light is the light visible without any light (mimics radiosity in the Real World)
	float4 ambient = lightAmbient * materialAmbient;

	// take the inverse of light's direction for dot product
    float3 dirToLight = -lightDirection.xyz;
    float3 normalInWorld = mul(input.normal, (float3x3)world);
	// find angle of light and clamp from 0-1 to (no such thing as negative light)
    float d = saturate(dot(normalInWorld, dirToLight));
	float4 diffuse = lightAmbient * materialAmbient;

    float3 dirToView = normalize(viewPosition.xyz - mul(input.position, world).xyz);
    float3 reflectedVector = reflect(lightDirection.xyz, normalInWorld);
    float s = saturate(dot(dirToView, reflectedVector));
    float4 specular = pow(s , materialPower) * lightSpecular * materialSpecular;

	// Specular power factor controls the sharpness of the specular reflection "(cos x)^specPower"

	output.color = specular + diffuse + ambient;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}