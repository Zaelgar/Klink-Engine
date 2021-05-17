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
		Colours::Colour grassColour;

		Colours::Colour rockColour;

		float grassThreshold;
		float padding1[3];

		float blendingAmount;
		float padding2[3];
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

	Klink::Graphics::TextureID mTerrainDiffuse;
	Klink::Graphics::TextureID mTerrainNormal;


	Klink::Graphics::MeshBuffer mCubeMesh;
	Klink::Graphics::TextureID mCubeDiffuse;
	Klink::Graphics::TextureID mCubeNormal;

	AppLog mAppLog;
};

#endif // #ifndef INCLUDED_GAMESTATE_H