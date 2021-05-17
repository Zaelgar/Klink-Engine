//====================================================================================================
// Filename:	Standard.fx
// Created by:	Jaidon!
// Description:	Default shader for Klink Engine. Supports Lighting, diffuse texturing, bump mapping, normal mapping, specular highlights
//====================================================================================================

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix wvp;
    matrix lightWvp;
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
    float4 materialSpecularPower;
}

cbuffer OptionsBuffer : register(b3)
{
    float4 grassColour;
    float4 rockColour;
    float slopeThreshold;
    float3 padding1;
    float blendAmount;
    float3 padding;
}

Texture2D normalMap : register(t1);

SamplerState textureSampler : register(s0);
SamplerState depthMapSampler : register(s1);

struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 position2 : TEXCOORD3;
};

VSOutput VS(VSInput input)
{
    float3 worldPosition = mul(float4(input.position.xyz, 1.0f), world).xyz;
    float3 worldNormal = mul(input.normal, (float3x3) world);
    float3 worldTangent = mul(input.tangent, (float3x3) world);
    float3 worldBinormal = cross(worldNormal, worldTangent);
	
	
	// Pass downward through rasterizer into pixel shader for per-pixel lighting. (Phong shading)
    VSOutput output;
    output.position = mul(input.position, wvp);
    output.normal = worldNormal;
    output.tangent = worldTangent;
    output.binormal = worldBinormal;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition.xyz - mul(input.position, world).xyz);
    output.texCoord = input.texCoord;
    output.position2 = mul(input.position, lightWvp);
    return output;
}

float4 PS(VSOutput input) : SV_Target
{	
	// Get normal from texture and convert from [0, 1] to [-1, 1]
    float3 sampledNormal = normalMap.Sample(textureSampler, input.texCoord).xyz;
    float3 unpackedNormal = (sampledNormal * 2) - 1;
	
	// Fix normals from rasterizer and construct the tangent space matrix
    float3 n = normalize(input.normal);
    float3 b = normalize(input.binormal);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);
    
    // True normal to use for lighting
    float3 normal = mul(unpackedNormal, tbnw);
    
    float4 ambient = lightAmbient * materialAmbient;
    
    float3 dirToLight = input.dirToLight;
    float diffuseIntensity = saturate(dot(normal, dirToLight));
    float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;
    
    float4 finalColour;
    
    //finalColour.xyz = normal.xyz;
    //finalColour.w = 1.0f;
    //return finalColour;
    
    //float slope = 1.0f - normal.y;
    float slope = 1.0f - saturate(dot(normal, float3(0.f, 1.f, 0.f)));
    float grassBlendHeight = slopeThreshold * (1.0f - blendAmount);
    float grassWeight = 1.0f - saturate((slope - grassBlendHeight) / (slopeThreshold - grassBlendHeight));
    float4 colour = grassColour * grassWeight + rockColour * (1.0f - grassWeight);
    
    finalColour = (ambient + diffuse) * colour;
    return finalColour;
}