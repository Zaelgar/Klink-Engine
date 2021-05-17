#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	void RenderScene();
	void PostProcess();
	void RenderDepthMap();

private:

	// All
	bool mIsUsingLightCamera = false;
	Klink::Graphics::Camera mMainCamera;
	Klink::Graphics::Camera mLightCamera;

	Klink::Graphics::DirectionalLight mLight;
	Klink::Graphics::Material mMaterial;

	// Brick Cube
	float mCubeSize = 2.0f;
	Klink::JMath::Vector3 mCubePosition;
	Klink::JMath::Vector3 mCubeRotation;

	Klink::Graphics::Mesh mCube;
	Klink::Graphics::MeshBuffer mCubeBuffer;

	Klink::Graphics::TextureID mCubeDiffuseTexture;
	Klink::Graphics::TextureID mCubeNormalTexture;
	Klink::Graphics::TextureID mCubeSpecularTexture;
	Klink::Graphics::TextureID mCubeBumpTexture;

	// Plane
	float mPlaneX = 30.0f;
	float mPlaneY = 30.0f;
	Klink::JMath::Vector3 mPlaneRotation;
	Klink::JMath::Vector3 mPlanePosition;

	Klink::Graphics::Mesh mPlane;
	Klink::Graphics::MeshBuffer mPlaneBuffer;

	Klink::Graphics::TextureID mPlaneDiffuseTexture;
	Klink::Graphics::TextureID mPlaneNormalTexture;
	Klink::Graphics::TextureID mPlaneSpecularTexture;
	Klink::Graphics::TextureID mPlaneBumpTexture;


	// Shader Buffers
	struct TransformData
	{
		Klink::JMath::Matrix4 world;
		Klink::JMath::Matrix4 wvp;
		Klink::JMath::Matrix4 lightWvp;
		Klink::JMath::Vector3 viewPosition;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		float padding[3];
	};
	using TransformBuffer = Klink::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Klink::Graphics::TypedConstantBuffer<Klink::Graphics::DirectionalLight>;
	using MaterialBuffer = Klink::Graphics::TypedConstantBuffer<Klink::Graphics::Material>;
	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;

	// DOES NOT WORK WITH CUBES (not enough vertices) Tesselation?
	float mDisplacementWeight = 0.0f;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;

	TypedConstantBuffer<Matrix4> mDepthBuffer;
	RenderTarget mDepthTarget;
};
#endif // INCLUDED_GAMESTATE_H