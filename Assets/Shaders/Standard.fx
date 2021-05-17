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
    float4 displacementWeight;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D displacementMap : register(t3);
Texture2D depthMap : register(t4);

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
    float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
    float4 position = input.position + (float4(input.normal, 0.2f) * displacement * displacementWeight.r);

    float3 worldPosition = mul(float4(position.xyz, 1.0f), world).xyz;
    float3 worldNormal = mul(input.normal, (float3x3) world);
    float3 worldTangent = mul(input.tangent, (float3x3) world);
    float3 worldBinormal = cross(worldNormal, worldTangent);
	
	
	// Pass downward through rasterizer into pixel shader for per-pixel lighting. (Phong shading)
    VSOutput output;
    output.position = mul(position, wvp);
    output.normal = worldNormal;
    output.tangent = worldTangent;
    output.binormal = worldBinormal;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition.xyz - mul(position, world).xyz);
    output.texCoord = input.texCoord;
    output.position2 = mul(position, lightWvp);
    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    float3 dirToView = normalize(input.dirToView);
	
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
    
    float3 dirToLight = -lightDirection;
    float diffuseIntensity = saturate(dot(normal, dirToLight));
    float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;
    
    float3 reflected = normalize(reflect(lightDirection, normal));
    float specularBase = saturate(dot(reflected, dirToView));
    float specularIntensity = pow(specularBase, materialSpecularPower);
    float4 specular = specularIntensity * lightSpecular * materialSpecular;
    
    float4 diffuseMapColour = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularMapColour = specularMap.Sample(textureSampler, input.texCoord);
    
    float realDepth = input.position2.z / input.position2.w;
    float2 shadowUV = input.position2.xy / input.position2.w;
    shadowUV = (shadowUV + 1.0f) * 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;

    float recordedDepth = depthMap.Sample(depthMapSampler, shadowUV).x;
    
    float4 finalColour;
    //if(usesShadow && realDepth < recordedDepth)
    /* SHADOW MAPPING V
    if (realDepth - 0.00001f > recordedDepth)
        finalColour = ambient * diffuseMapColour;
    else
*/
	finalColour = (ambient + diffuse) * diffuseMapColour + (specular * specularMapColour.r);
    return finalColour;
}