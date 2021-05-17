#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <../Engine/Klink/Inc/Klink.h>

class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	// All
	Klink::Graphics::Camera mCamera;

	Klink::JMath::Vector3 mPosition;
	Klink::JMath::Vector3 mRotation;

	Klink::Graphics::DirectionalLight mLight;
	Klink::Graphics::Material mMaterial;

	float mDisplacementWeight = 0.5f;
	float timeKeep = 0.0f;
	float sinModValue = 0.0f;


	// Space Box
	Klink::Graphics::VertexShader mTextureVertexShader;
	Klink::Graphics::PixelShader mTexturePixelShader;

	Klink::Graphics::BaseMesh<VertexPX> mSpaceBox;
	Klink::Graphics::MeshBuffer mSpaceBoxBuffer;

	Klink::Graphics::TextureID mSpaceBoxTexture;

	ConstantBuffer mConstantBuffer;

	// Earth
	Klink::Graphics::VertexShader mStandardVertexShader;
	Klink::Graphics::PixelShader mStandardPixelShader;

	Klink::Graphics::Mesh mEarth;
	Klink::Graphics::MeshBuffer mEarthBuffer;

	Klink::Graphics::TextureID mEarthDiffuseTexture;
	Klink::Graphics::TextureID mEarthSpecularTexture;
	Klink::Graphics::TextureID mEarthBumpTexture;
	Klink::Graphics::TextureID mEarthNormalTexture;


	// Shader Buffers
	struct TransformData
	{
		Klink::JMath::Matrix4 world;
		Klink::JMath::Matrix4 wvp;
		Klink::JMath::Vector3 viewPosition;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		float padding[3];
		float sinMod;
		float padding2[3];
	};
	using TransformBuffer = Klink::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Klink::Graphics::TypedConstantBuffer<Klink::Graphics::DirectionalLight>;
	using MaterialBuffer = Klink::Graphics::TypedConstantBuffer<Klink::Graphics::Material>;
	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;
};
#endif // INCLUDED_GAMESTATE_H#pragma once
