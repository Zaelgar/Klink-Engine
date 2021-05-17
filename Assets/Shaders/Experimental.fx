//====================================================================================================
// Filename:	Standard.fx
// Created by:	Jaidon!
// Description:	Default shader for Klink Engine. Supports Lighting, diffuse texturing, bump mapping, normal mapping, specular highlights
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

cbuffer OptionsBuffer : register(b3)
{
    float4 displacementWeight;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D bumpMap : register(t2);
Texture2D normalMap : register(t3);

SamplerState textureSampler : register(s0);

struct VSInput
{
	float4 position	    : POSITION;
	float3 normal	    : NORMAL;
	float3 tangent	    : TANGENT;
	float2 texCoord     : TEXCOORD0;
};

struct VSOutput
{
	float4 position		: SV_POSITION;
    float3 normal		: NORMAL;
    float3 tangent      : TANGENT;
    float3 dirToLight	: TEXCOORD1;
    float3 dirToView	: TEXCOORD2;
    float2 texCoord		: TEXCOORD3;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

    float displacement = bumpMap.SampleLevel(textureSampler, input.texCoord, 0).x;
    float4 position = input.position + (float4(input.normal, 0.0f) * displacement * displacementWeight.r);

	// Pass downward through rasterizer into pixel shader for per-pixel lighting. (Phong shading)
    output.position = mul(position, wvp);
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.bitangent = cross(output.normal, output.tangent);    // Already normalized?
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition.xyz - mul(position, world).xyz);
    output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	// Regardless of lights, the ambient light is the light visible without any light (mimics ambient light in the Real World)
    float3 normal = normalize(input.normal);
    float3 tangent = normalize(input.tangent);
    float3 binormal = normalize(input.binormal);
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);
    
	float4 ambient = lightAmbient * materialAmbient;

    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

    float3 r = reflect(lightDirection.xyz, normal);
    float s = saturate(dot(dirToView, r));
    float4 specular = pow(s, materialPower.x) * lightSpecular * materialSpecular;

    // Bump Mapping
    float4 bumpValue = bumpMap.Sample(textureSampler, input.texCoord);
    bumpValue = (bumpValue * 2.0f) - 1.0f;
    float3 bumpNormal = (bumpValue.x * input.tangent) + (bumpValue.y * input.bitangent) + (bumpValue.z * input.normal);
    bumpNormal = normalize(bumpNormal);

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularColor = specularMap.Sample(textureSampler, input.texCoord);

    return (((ambient + diffuse) * diffuseColor) + (specular * specularColor.r)).xyzz * (saturate(dot(bumpNormal, lightDirection)));
}