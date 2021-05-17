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

	Klink::Graphics::VertexShader mVertexColourShader;
	Klink::Graphics::PixelShader mPixelColourShader;
	Klink::Graphics::TypedConstantBuffer<Matrix4> mConstantBuffer;

	Klink::Graphics::MeshPC mCube;
	Klink::Graphics::MeshBuffer mCubeBuffer;
};
#endif // INCLUDED_GAMESTATE_H