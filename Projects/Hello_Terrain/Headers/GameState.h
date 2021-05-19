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

private:

	void BuildTerrain(int type);
	void ErodeTerrain(int numIterations);

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
		float padding0[3];

		float lowSlopeThreshold;
		float padding1[3];

		float lowScaling;       // UV Scaling factor for low texture
		float padding2[3];

		// Mid settings
		float midHeightLimit;   // From lowHeightLimit -> this number
		float padding3[3];

		float midSlopeThreshold;
		float padding4[3];

		float midScaling;       // UV Scaling factor for mid texture
		float padding5[3];

		// High setting
		float highSlopeThreshold;
		float padding6[3];

		float highScaling;      // UV Scaling factor for high texture
		float padding7[3];
	};
	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;
	OptionsBuffer mOptionsBuffer;

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

	float mHeightScale = 6.0f;
	float mScale = 20.0f;

	Colours::Colour mGrassColour = { 0.309f, 0.341f, 0.0f, 1.0f };
	Colours::Colour mRockColour = { 0.235f, 0.113f, 0.07f, 1.0f };
	float mGrassThreshold = 0.18f;
	float mBlendingAmount = 0.15f;

	// Debug and Settings
	bool mCanRenderTerrain = false;
	int mNumErosionIterations = 70000;

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
	Klink::Graphics::MeshBuffer mCubeMesh;
	Klink::Graphics::TextureID mCubeDiffuse;

	AppLog mAppLog;
};

#endif // #ifndef INCLUDED_GAMESTATE_H