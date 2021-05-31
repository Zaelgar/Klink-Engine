#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>
#include <imgui/Inc/AppLog.h>
#include <ctime>

#include "Erosion.h"
#include "Heightmap.h"

class GameState : public Klink::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void RenderScene();
	void RenderDepthMap();

private:

	void BuildTerrain(int type);
	void ErodeTerrain(int numIterations);
	void ResetDayCycle();

	Klink::Graphics::Camera mCamera;

	struct TransformData
	{
		Matrix4 world;
		Matrix4 wvp;
		Vector3 viewPosition;
		float padding;
	};
	using TransformBuffer = Klink::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;


	struct OptionsData
	{
		// Low settings
		float lowHeightLimit;   // From 0 -> this number
		float lowSlopeThreshold;
		float lowScaling;       // UV Scaling factor for low texture
		float padding0;

		// Mid settings
		float midHeightLimit;   // From lowHeightLimit -> this number
		float midSlopeThreshold;
		float midScaling;       // UV Scaling factor for mid texture
		float padding1;

		// High setting
		float highHeightLimit;
		float highSlopeThreshold;
		float highScaling;      // UV Scaling factor for high texture
		float padding2;

		float blendingAmount;
		float sinFactor = 0.3f;
		float snowHeightFactor = 1.0f;
		float padding3;
	};
	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;
	OptionsBuffer mOptionsBuffer;

	float mSinFactor = 0.3f;
	float mSnowHeightFactor = 1.0f;

	// Lights
	Klink::Graphics::DirectionalLight mDirectionalLight;
	using LightBuffer = Klink::Graphics::TypedConstantBuffer<DirectionalLight>;
	LightBuffer mLightBuffer;

	// Materials
	Klink::Graphics::Material mMaterial;
	using MaterialBuffer = Klink::Graphics::TypedConstantBuffer<Material>;
	MaterialBuffer mMaterialBuffer;


	// Terrain Visualization
	int mIterationsPerUpdate = 2;
	bool mIsVisualised = false;

	// Terrain
	std::string mFilename;
	int mMapType = 1;

	Klink::Terrain::Heightmap mHeightmap;
	Klink::Terrain::Erosion mErosion;
	Klink::Terrain::Erosion::ErosionSettings mErosionSettings = {};

	float mHeightScale = 10.0f;
	float mScale = 20.0f;

	float mBlendingAmount = 0.1f;

	// Debug and Settings
	bool mCanRenderTerrain = false;
	int mNumErosionIterations = 200000;

	bool mIsRenderingNormals = false;

	bool mIsWireframe = false;
	Vector3 mTerrainPosition = Vector3::Zero();

	Klink::Graphics::MeshBuffer mTerrainMeshBuffer;
	Klink::Graphics::Mesh mTerrainMesh;

	float mLowHeightLimit;
	Klink::Graphics::TextureID mTerrainLowDiffuse;
	Klink::Graphics::TextureID mTerrainLowNormal;

	float mMidHeightLimit;
	Klink::Graphics::TextureID mTerrainMidDiffuse;
	Klink::Graphics::TextureID mTerrainMidNormal;

	float mHighHeightLimit;
	Klink::Graphics::TextureID mTerrainHighDiffuse;
	Klink::Graphics::TextureID mTerrainHighNormal;


	// Skycube
	struct SkyboxData
	{
		Matrix4 wvp;
	};
	using SkyboxBuffer = Klink::Graphics::TypedConstantBuffer<SkyboxData>;
	SkyboxBuffer mSkyboxBuffer;
	Klink::Graphics::MeshBuffer mSkyboxMeshBuffer;
	Klink::Graphics::TextureID mSkyboxDiffuse;
	
	// Day cycle
	struct SkyboxOptions
	{
		Klink::Graphics::Colours::Colour colourTint;
	};

	using SkyboxOptionsBuffer = Klink::Graphics::TypedConstantBuffer<SkyboxOptions>;
	SkyboxOptionsBuffer mSkyboxOptionsBuffer;
	float mTintPercent = 0.1f;
	Klink::Graphics::Colours::Colour mTintColour = {};

	bool mIsDayCycling = false;
	float mDaySpeed = 0.1f;
	float mDayTimeValue = 0.0f;
	Klink::Graphics::Colours::Colour mSunrise = {1.0f, 0.90f, 0.30f, 1.0f}; // more yellowy
	Klink::Graphics::Colours::Colour mSunset = {1.0f, 0.66f, 0.2f, 1.0f}; // orangy
	Klink::Graphics::Colours::Colour mDay = {1.0f, 1.0f, 1.0f, 1.0f}; // white

	// Shadow Mapping
	Klink::Graphics::Camera mLightCamera;

	Klink::Graphics::TypedConstantBuffer<Matrix4> mDepthBuffer;
	Klink::Graphics::RenderTarget mDepthTarget;

	AppLog mAppLog;
};

#endif // #ifndef INCLUDED_GAMESTATE_H