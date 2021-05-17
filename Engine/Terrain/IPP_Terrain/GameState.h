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

	Klink::Graphics::Camera mCamera;
	Klink::Graphics::MeshPX mSphere;
	Klink::Graphics::MeshBuffer mSphereBuffer;

	Klink::Graphics::VertexShader mVertexShader;
	Klink::Graphics::PixelShader mPixelShader;
	Klink::Graphics::ConstantBuffer mConstantBuffer;
	Klink::Graphics::Texture mTexture;

	Klink::JMath::Matrix4 mTransform{ Klink::JMath::Matrix4::Identity() };
};
#endif // INCLUDED_GAMESTATE_H