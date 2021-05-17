#include "Precompiled.h"
#include "Sprite.h"

using namespace Klink::Graphics;

Sprite::Sprite()
{}

void Sprite::Initialize()
{
}

void Sprite::Terminate()
{
}

TextureID Sprite::AddSprite(std::string name, std::filesystem::path path, SpriteDimensions spriteDimensions)
{
	auto[iter, success] = mTextures.try_emplace(name, TextureManager::Get()->LoadTexture(path));
	if (success)
	{
		mSpriteDimensions = spriteDimensions;
		return iter->second;
	}
	return 0;
}

TextureID Sprite::GetSprite(const std::string& name)
{
	auto iter = mTextures.find(name);
	if (iter == mTextures.end())
		return 0;
	
	return iter->second;
}