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
	Klink::JMath::Quaternion mRotation;
	Klink::JMath::Vector3 mRotationEuler;
	Klink::JMath::Vector3 mScale;


	// Space Box
	Klink::Graphics::VertexShader mTextureVertexShader;
	Klink::Graphics::PixelShader mTexturePixelShader;

	Klink::Graphics::TextureID mZombieDiffuseOne;
	Klink::Graphics::TextureID mZombieDiffuseTwo;

	// Shader Buffers
	struct TransformData
	{
		Klink::JMath::Matrix4 wvp;
	};

	using TransformBuffer = Klink::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;

	struct OptionsData
	{
		float uTranslate;
		float vTranslate;
		float padding[2];
	};

	using OptionsBuffer = Klink::Graphics::TypedConstantBuffer<OptionsData>;
	OptionsBuffer mOptionsBuffer;

	float uTranslate = 0.0f;
	float vTranslate = 0.0f;

	Model zombieModel;
};
#endif // INCLUDED_GAMESTATE_H