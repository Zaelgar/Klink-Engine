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
	
	Vector3 mPosition;
	Quaternion mRotation;
	Vector3 mRotationEuler;
	Vector3 mScale;

	Model mZombieModel;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	// Shader Buffers
	DirectionalLight mDirectionalLight;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	LightBuffer mLightBuffer;

	Material mMaterial;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	MaterialBuffer mMaterialBuffer;

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
		float displacementWeight;
		float padding[3];
	};
	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;
	OptionsBuffer mOptionsBuffer;
};
#endif // INCLUDED_GAMESTATE_H