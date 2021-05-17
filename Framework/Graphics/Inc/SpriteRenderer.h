#ifndef INCLUDED_KLINK_GRAPHICS_SPRITERENDERER_H
#define INCLUDED_KLINK_GRAPHICS_SPRITERENDERER_H

#include "JMath/Inc/JMath.h"

namespace DirectX { class SpriteBatch; }

namespace Klink::Graphics
{
	class Texture;

class SpriteRenderer
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SpriteRenderer* Get();

public:
	SpriteRenderer() = default;
	~SpriteRenderer();

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer) = delete;

	void Initialize();

	void BeginRender();
	void EndRender();

	void Draw(const Texture& texture, const JMath::Vector2& position);

	void Terminate();

private:

	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_SPRITERENDERER_H