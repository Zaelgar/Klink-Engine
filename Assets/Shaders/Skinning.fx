//====================================================================================================
// Filename:	Skinning.fx
// Created by:	Jaidon!
// Description:	Shader for skinning animated meshes
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

cbuffer BoneTransformBuffer : register(b4)
{
    matrix boneTransforms[256]; // was 128 model has 210 bones
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D displacementMap : register(t3);

SamplerState textureSampler : register(s0);

struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
};

static matrix Identity =
{
    1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
    if (length(weights) <= 0.0f)
    {
        return Identity;
    }

	// Naive way of doing this. Called "Linear Skinning". Some artifacts may be present
    matrix transform;
    transform = boneTransforms[indices[0]] * weights[0];
    transform += boneTransforms[indices[1]] * weights[1];
    transform += boneTransforms[indices[2]] * weights[2];
    transform += boneTransforms[indices[3]] * weights[3];
    
    return Identity; // Testing purposes... should be "return transform;"
}

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;

    float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
    float4 bonePosition = input.position; // + (float4(input.normal, 0.0f) * displacement * displacementWeight.r);

    matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
    matrix toWorld = boneTransform * world;
    matrix toNDC = boneTransform * wvp;// was bonePosition

    float3 normal = mul(input.normal, (float3x3)toWorld);
    float3 tangent = mul(input.tangent, (float3x3)toWorld);
    float3 binormal = cross(normal, tangent);

	// Pass downward through rasterizer into pixel shader for per-pixel lighting. (Phong shading)
    output.position = mul(input.position, wvp); // Testing purposes... should be "mul(bonePosition, toNDC);"
    output.normal = normal;
    output.tangent = tangent;
    output.bitangent = binormal;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition.xyz - mul(bonePosition, world).xyz);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);

    float3 n = normalize(input.normal);
    float3 b = normalize(input.bitangent);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);

    float3 normal = normalize(mul(sampledNormal, tbnw));
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);
    
    float4 ambient = lightAmbient * materialAmbient;

    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

	//float3 r = reflect(lightDirection, normal);
	//float s = saturate(dot(dirToView, r));
    float3 h = (dirToLight + dirToView) * 0.5f;
    float s = saturate(dot(h, normal));
    float4 specular = pow(s, materialPower.x) * lightSpecular * materialSpecular;


    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularColor = specularMap.Sample(textureSampler, input.texCoord);

    return ((ambient + diffuse) * diffuseColor) + (specular * specularColor.r);
}