//====================================================================================================
// Filename:	TextureTerrain.fx
// Created by:	Jaidon
// Description:	Texture shader that uses height and slope values to sample different texture maps
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
    float lowHeightLimit;
    float3 padding0;
    float lowSlopeThreshold;
    float3 padding1;
    float lowScaling;
    float3 padding2;
    float midHeightLimit;
    float3 padding3;
    float midSlopeThreshold;
    float3 padding4;
    float midScaling;
    float3 padding5;
    float highSlopeThreshold;
    float3 padding6;
    float highScaling;
    float3 padding7;
}

Texture2D lowAlbedo : register(t0);
Texture2D lowNormal : register(t1);

Texture2D midAlbedo : register(t2);
Texture2D midNormal : register(t3);

Texture2D highAlbedo : register(t4);
Texture2D highNormal : register(t5);

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
    float4 position3 : TEXCOORD4;
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
    output.position3 = input.position;
    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    // Get normal from texture and convert from [0, 1] to [-1, 1]
    //float3 sampledNormal = normalMap.Sample(textureSampler, input.texCoord).xyz;
	
	// Fix normals from rasterizer and construct the tangent space matrix
    float3 n = normalize(input.normal);
    float3 b = normalize(input.binormal);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);
    
    
    
    
    
    float3 sampledNormal;
    float4 diffuseMapColour;
    float4 finalCol;
    
    if (input.position3.y <= lowHeightLimit) // moss
    {
        // Get normal from texture and convert from [0, 1] to [-1, 1]
        sampledNormal = lowNormal.Sample(textureSampler, input.texCoord).xyz;
        diffuseMapColour = lowAlbedo.Sample(textureSampler, input.texCoord);
    }
    else if (input.position3.y <= midHeightLimit) // rock
    {
        sampledNormal = midNormal.Sample(textureSampler, input.texCoord).xyz;
        diffuseMapColour = midAlbedo.Sample(textureSampler, input.texCoord);
    }
    else
    {
        sampledNormal = highNormal.Sample(textureSampler, input.texCoord).xyz;
        diffuseMapColour = highAlbedo.Sample(textureSampler, input.texCoord);
    }
    
    // ---------------------------------------------------
    float3 unpackedNormal = (sampledNormal * 2) - 1;
    // True normal to use for lighting
    float3 normal = mul(unpackedNormal, tbnw);
    
    float4 ambient = lightAmbient * materialAmbient;
    
    float3 dirToLight = input.dirToLight;
    float diffuseIntensity = saturate(dot(normal, dirToLight));
    float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;
    
    finalCol = (ambient + diffuse) * diffuseMapColour;
    
    return finalCol;
}