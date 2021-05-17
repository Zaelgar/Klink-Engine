#include "Precompiled.h"
#include "SpriteRenderer.h"

#include "D3DUtilities.h"
#include "GraphicsSystem.h"
#include "Texture.h"
#include <DirectXTK/Inc/SpriteBatch.h>

using namespace Klink::Graphics;
using namespace Klink::JMath;

namespace
{
	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initialized!");

	sSpriteRenderer = std::make_unique<SpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void Klink::Graphics::SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

SpriteRenderer* SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] Sprite renderer is not initialized!");
	return sSpriteRenderer.get();
}

SpriteRenderer::~SpriteRenderer()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Terminate was not called correctly!");
}

void SpriteRenderer::Initialize()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Already Initialized!");
	auto context = GetDeviceContext();
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

void SpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not Initialized!");
	mSpriteBatch->Begin();
}

void SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not Initialized!");
	mSpriteBatch->End();
}

void SpriteRenderer::Draw(const Texture & texture, const Vector2 & position)
{
	mSpriteBatch->Draw(GetShaderResourceView(texture), DirectX::XMFLOAT2{ position.x, position.y });
}

void SpriteRenderer::Terminate()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Already terminated!");
	mSpriteBatch.reset();
}