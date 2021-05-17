#include "Precompiled.h"
#include "TextureManager.h"

using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(std::filesystem::path rootPath)
{
	ASSERT(sTextureManager == nullptr, "[SpriteRenderer] System already initialized!");

	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->Initialize();
	sTextureManager->SetRootPath(std::move(rootPath));
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Terminate();
		sTextureManager.reset();
	}
}

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[TextureManager] System is not initialized!");
	return sTextureManager.get();
}

Klink::Graphics::TextureManager::~TextureManager()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

void Klink::Graphics::TextureManager::Initialize()
{

}

void Klink::Graphics::TextureManager::Terminate()
{

}

void Klink::Graphics::TextureManager::SetRootPath(std::filesystem::path path)
{
	ASSERT(std::filesystem::is_directory(path), "[TextureManager] %ls is not a directory!", path.c_str());
	mRootPath = std::move(path);
}

void Klink::Graphics::TextureManager::UseRootPath(bool usesRootPath)
{
	useRootPath = usesRootPath;
}

TextureID Klink::Graphics::TextureManager::LoadTexture(std::filesystem::path fileName)
{
	auto hash = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({hash, nullptr});
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize(useRootPath ? mRootPath / fileName : fileName);
	}
	return hash;
}

const Texture* Klink::Graphics::TextureManager::GetTexture(TextureID textureId) const
{
	auto iter = mInventory.find(textureId);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}