//====================================================================================================
// Filename:	Blur.fx
// Created by:	Jaidon van Herwaarden
// Description:	Simple Blur Post-Processing
//====================================================================================================

Texture2D renderTarget : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position	: POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
    float2 texCoordUp : TEXCOORD2;
    float2 texCoordDown : TEXCOORD3;
    float2 texCoordLeft : TEXCOORD4;
    float2 texCoordRight : TEXCOORD5;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = input.position;
	output.texCoord = input.texCoord;

    // Get dimensions of texture (should be same as rendertarget)
    uint widthTexel = 1024;
    uint heightTexel = 576;
    //renderTarget.GetDimensions(widthTexel, heightTexel);

    // texel coordinates based of of UV 0-1 and dimensions of texture
    //float widthTexel = 1.0f / width;
    //float heightTexel = 1.0f / height;

    // adjacent pixels off of texture
    output.texCoordUp = input.texCoord + float2(0.0f, heightTexel * -1.0f);
    output.texCoordDown = input.texCoord + float2(0.0f, heightTexel * 1.0f);
    output.texCoordLeft = input.texCoord + float2(widthTexel * -1.0f, 0.0f);
    output.texCoordRight = input.texCoord + float2(widthTexel * 1.0f, 0.0f);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 endPixel = float4(0.0f, 0.0f, 0.0f, 1.0f);

    endPixel += renderTarget.Sample(textureSampler, input.texCoord);
    endPixel += renderTarget.Sample(textureSampler, input.texCoordUp) * 0.5f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordDown) * 0.5f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordLeft) * 0.5f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordRight) * 0.5f;

    endPixel /= 4.0f;
    endPixel.a = 1.0f;

    return endPixel;

    //return renderTarget.Sample(textureSampler, input.texCoord);
}