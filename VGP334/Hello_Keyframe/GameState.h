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

	// All
	Klink::Graphics::Camera mCamera;
	
	Klink::JMath::Vector3 mPosition;
	Klink::JMath::Vector3 mRotationEuler;
	Klink::JMath::Quaternion mRotation;
	Klink::JMath::Vector3 mScale = Vector3::One();

	Klink::JMath::Matrix4 mTransform;


	// Cube render
	Klink::Graphics::VertexShader mTextureVertexShader;
	Klink::Graphics::PixelShader mTexturePixelShader;

	Klink::Graphics::TextureID mCubeTexture;

	Klink::Graphics::MeshPX mCube;
	Klink::Graphics::MeshBuffer mCubeBuffer;

	// Shader Buffers
	struct TransformData
	{
		Klink::JMath::Matrix4 wvp;
	};

	using TransformBuffer = Klink::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;


	// Animation
	Animation cubeAnimation;
	AnimationBuilder animBuilder;
	Animator animator;
	float mTime = 0.0f;

};
#endif // INCLUDED_GAMESTATE_H