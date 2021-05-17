// Description: Shader for recording pixel depths.

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

// If you want to use displacement map (bump map), you need
// to add a SettingsBuffer here so you can control the
// displacement scale.

Texture2D displacementMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 position2 : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 position = input.position + (input.normal * displacement * 0.0f);

    VS_OUTPUT output;
    output.position = mul(float4(position, 1.0f), wvp);
    output.position2 = mul(float4(position, 1.0f), wvp);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float depth = input.position2.z / input.position2.w;
    return float4(depth, depth, depth, 1.0f);
}