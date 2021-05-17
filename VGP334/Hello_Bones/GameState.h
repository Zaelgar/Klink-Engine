#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;

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
	Camera mCamera;


	Model mZombieModel;
	AnimationClip mZombieIdleAnimation;
	Animator mZombieAnimator;
	std::vector<Matrix4> boneMatrices;

	float t = 0.0f;
	bool mShowSkeleton = true;

	/// Shaders and buffers
	VertexShader mVertexShader;
	PixelShader mPixelShader;

	struct TransformData
	{
		Matrix4 world;
		Matrix4 wvp;
		Vector3 viewPosition;
		float padding;
	};
	using TransformBuffer = TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;

	DirectionalLight mDirectionalLight;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	LightBuffer mLightBuffer;

	Material mMaterial;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	MaterialBuffer mMaterialBuffer;
	
	struct OptionsData
	{
		float displacementWeight;
		float padding[3];
	};
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;
	OptionsBuffer mOptionsBuffer;

	struct BoneTransformData
	{
		Matrix4 boneTransforms[256];
	};
	using BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;
	BoneTransformBuffer mBoneTransformBuffer;	
};
#endif // INCLUDED_GAMESTATE_H