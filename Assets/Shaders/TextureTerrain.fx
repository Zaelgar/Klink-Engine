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
    float lowHeightLimit : packoffset(c0.x);
    float lowSlopeThreshold : packoffset(c0.y);
    float lowScaling : packoffset(c0.z);
    
    float midHeightLimit : packoffset(c1.x);
    float midSlopeThreshold : packoffset(c1.y);
    float midScaling : packoffset(c1.z);
    
    float highHeightLimit : packoffset(c2.x);
    float highSlopeThreshold : packoffset(c2.y);
    float highScaling : packoffset(c2.z);
    
    float blendingAmount : packoffset(c3.x);
    float sinFactor : packoffset(c3.y);
    float snowHeightFactor : packoffset(c3.z);
}

Texture2D lowAlbedo : register(t0);
Texture2D lowNormal : register(t1);

Texture2D midAlbedo : register(t2);
Texture2D midNormal : register(t3);

Texture2D highAlbedo : register(t4);
Texture2D highNormal : register(t5);

Texture2D depthMap : register(t6);

SamplerState textureSampler : register(s0);
SamplerState depthMapSampler : register(s1);

struct BlendingData
{
    float height;
    float4 result;
};

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
	// Fix normals from rasterizer and construct the tangent space matrix
    float3 n = normalize(input.normal);
    float3 b = normalize(input.binormal);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);
    
    float3 lowSampledNormal;
    float3 midSampledNormal;
    float3 highSampledNormal;
    
    float4 lowDiffuseMapColour;
    float4 midDiffuseMapColour;
    float4 highDiffuseMapColour;
    
    float2 lowScaledUV;
    float2 midScaledUV;
    float2 highScaledUV;
    
    // Low texture
    lowScaledUV = mul(input.texCoord, lowScaling);
    lowScaledUV = lowScaledUV % 1.0f;

    lowSampledNormal = lowNormal.Sample(textureSampler, lowScaledUV).xyz;
    lowDiffuseMapColour = lowAlbedo.Sample(textureSampler, lowScaledUV);
    
    // Mid
    midScaledUV = mul(input.texCoord, midScaling);
    midScaledUV = midScaledUV % 1.0f;
        
    midSampledNormal = midNormal.Sample(textureSampler, midScaledUV).xyz;
    midDiffuseMapColour = midAlbedo.Sample(textureSampler, midScaledUV);
    
    // High
    highScaledUV = mul(input.texCoord, highScaling);
    highScaledUV = highScaledUV % 1.0f;
        
    highSampledNormal = highNormal.Sample(textureSampler, highScaledUV).xyz;
    highDiffuseMapColour = highAlbedo.Sample(textureSampler, highScaledUV);
    
    float amount;
    float4 finalDiffuseColour = lowDiffuseMapColour;
    float3 sampledNormal = lowSampledNormal;
    
    float snowHeightOffset = (sin(sinFactor * input.position3.x) * snowHeightFactor); // change snow height based on a sinwave using the x position of the vertex in local space.
    
    float inputPosition = input.position3.y;
    float midHeight = midHeightLimit - snowHeightOffset;
    
    if (inputPosition > midHeight)
    {
        amount = clamp((inputPosition - midHeight) / (midHeight * blendingAmount), 0.0f, 1.0f);
        finalDiffuseColour = lerp(midDiffuseMapColour, highDiffuseMapColour, amount);
        sampledNormal = lerp(midSampledNormal, highSampledNormal, amount);
    }
    //if (input.position3.y > midHeightLimit)
    //{
    //    amount = clamp((input.position3.y - midHeightLimit) / (midHeightLimit * blendingAmount), 0.0f, 1.0f);
    //    finalDiffuseColour = lerp(midDiffuseMapColour, highDiffuseMapColour, amount);
    //    sampledNormal = lerp(midSampledNormal, highSampledNormal, amount);
    //}
    else // TODO - use slope based rendering for moss and stone
    {
        amount = clamp((input.position3.y - lowHeightLimit) / ((midHeightLimit - lowHeightLimit) * blendingAmount), 0.0f, 1.0f);
        finalDiffuseColour = lerp(lowDiffuseMapColour, midDiffuseMapColour, amount);
        sampledNormal = lerp(lowSampledNormal, midSampledNormal, amount);
    }
    
    // ---------------------------------------------------
    float3 unpackedNormal = (sampledNormal * 2) - 1;
    // True normal to use for lighting
    float3 normal = mul(unpackedNormal, tbnw);
    
    float4 ambient = lightAmbient * materialAmbient;
    
    float3 dirToLight = input.dirToLight;
    float diffuseIntensity = saturate(dot(normal, dirToLight));
    float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;
    
    // Shadow Mapping
    float realDepth = input.position2.z / input.position2.w;
    float2 shadowUV = input.position2.xy / input.position2.w;
    shadowUV = (shadowUV + 1.0f) * 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
    
    float recordedDepth = depthMap.Sample(depthMapSampler, shadowUV).x;
    
    float4 finalCol;
    
    if (realDepth - 0.00001f > recordedDepth)
        finalCol = ambient * finalDiffuseColour;
    else
        finalCol = (ambient + diffuse) * finalDiffuseColour;
    
    return finalCol;
}