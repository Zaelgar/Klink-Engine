#ifndef INCLUDED_KLINK_GRAPHICS_SPRITE_H
#define INCLUDED_KLINK_GRAPHICS_SPRITE_H

#include "TextureManager.h"

namespace Klink::Graphics
{

struct SpriteDimensions
{
	int width;
	int height;
	int widthOffset;
	int heightOffset;
};

class Sprite
{
public:

	Sprite();

	void Initialize();
	void Terminate();

	TextureID AddSprite(std::string name, std::filesystem::path path, SpriteDimensions spriteDimensions);
	TextureID GetSprite(const std::string& name);

private:

	SpriteDimensions mSpriteDimensions;
	std::unordered_map<std::string, TextureID> mTextures;
};
} // namespace Klink::Graphics

#endif //INCLUDED_KLINK_GRAPHICS_SPRITE_H