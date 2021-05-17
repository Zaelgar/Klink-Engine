#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>
#include <Physics/Inc/Physics.h>

class GameApp : public Engine::Core::Application
{
public:
	GameApp() = default;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	struct ConstantData
	{
		Engine::Math::Matrix wvp;
		Engine::Math::Vector4 viewPosition;
		Engine::Math::Vector3 viewUp;
		float padding;
	};

	Engine::Core::Window mWindow;
	
	Engine::Graphics::Camera mCamera;

	Engine::Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;

	Engine::Graphics::VertexShader mVertexShader;
	Engine::Graphics::GeometryShader mGeometryShader;
	Engine::Graphics::PixelShader mPixelShader;

	std::array<Engine::Graphics::BlendState, 4> mBlendStates;
	Engine::Graphics::Sampler mSampler;
	Engine::Graphics::Texture mTexture;

	Engine::Graphics::MeshBuffer mMeshBuffer;
	int mBlendStateSelected = 0;
};

#endif // #ifndef INCLUDED_GAMEAPP_H