//====================================================================================================
// Filename:	Blur.fx
// Created by:	Jaidon van Herwaarden
// Description:	Simple Blur Post-Processing
//====================================================================================================

cbuffer PostOptionsBuffer : register(b0)
{
    float4 sampleHeight;
    float4 sampleWidth;
}

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

    float2 texCoordUp1 : TEXCOORD2;
    float2 texCoordUp2 : TEXCOORD3;
    float2 texCoordUp3 : TEXCOORD4;

    float2 texCoordDown1 : TEXCOORD5;
    float2 texCoordDown2 : TEXCOORD6;
    float2 texCoordDown3 : TEXCOORD7;

    float2 texCoordLeft1 : TEXCOORD8;
    float2 texCoordLeft2 : TEXCOORD9;
    float2 texCoordLeft3 : TEXCOORD10;

    float2 texCoordRight1 : TEXCOORD11;
    float2 texCoordRight2 : TEXCOORD12;
    float2 texCoordRight3 : TEXCOORD13;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = input.position;
	output.texCoord = input.texCoord;

    // Get dimensions of texture (should be same as rendertarget)

    //renderTarget.GetDimensions()

    /*
    float widthTexel = 1280;
    float heightTexel = 720;
    widthTexel = 1.0f / widthTexel;
    heightTexel = 1.0f / heightTexel;
    */

    float widthTexel = 1.0f / sampleWidth;
    float heightTexel = 1.0f / sampleHeight;

    // adjacent pixels off of texture
    output.texCoordUp1 = input.texCoord + float2(0.0f, heightTexel * -1.0f);
    output.texCoordUp2 = input.texCoord + float2(0.0f, heightTexel * -2.0f);
    output.texCoordUp3 = input.texCoord + float2(0.0f, heightTexel * -3.0f);

    output.texCoordDown1 = input.texCoord + float2(0.0f, heightTexel * 1.0f);
    output.texCoordDown2 = input.texCoord + float2(0.0f, heightTexel * 2.0f);
    output.texCoordDown3 = input.texCoord + float2(0.0f, heightTexel * 3.0f);

    output.texCoordLeft1 = input.texCoord + float2(widthTexel * -1.0f, 0.0f);
    output.texCoordLeft2 = input.texCoord + float2(widthTexel * -2.0f, 0.0f);
    output.texCoordLeft3 = input.texCoord + float2(widthTexel * -3.0f, 0.0f);

    output.texCoordRight1 = input.texCoord + float2(widthTexel * 1.0f, 0.0f);
    output.texCoordRight2 = input.texCoord + float2(widthTexel * 2.0f, 0.0f);
    output.texCoordRight3 = input.texCoord + float2(widthTexel * 3.0f, 0.0f);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 endPixel = float4(0.0f, 0.0f, 0.0f, 1.0f);

    endPixel += renderTarget.Sample(textureSampler, input.texCoord);

    endPixel += renderTarget.Sample(textureSampler, input.texCoordUp1) * 0.8f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordUp2) * 0.6f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordUp3) * 0.4f;

    endPixel += renderTarget.Sample(textureSampler, input.texCoordDown1) * 0.8f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordDown2) * 0.6f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordDown3) * 0.4f;

    endPixel += renderTarget.Sample(textureSampler, input.texCoordLeft1) * 0.8f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordLeft2) * 0.6f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordLeft3) * 0.4f;

    endPixel += renderTarget.Sample(textureSampler, input.texCoordRight1) * 0.8f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordRight2) * 0.6f;
    endPixel += renderTarget.Sample(textureSampler, input.texCoordRight3) * 0.4f;

    endPixel /= 12;

    endPixel.a = 1.0f;

    return endPixel;

    //return renderTarget.Sample(textureSampler, input.texCoord);
}