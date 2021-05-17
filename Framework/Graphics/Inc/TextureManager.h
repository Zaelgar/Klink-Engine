#ifndef INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H
#define INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H

#include "Texture.h"

namespace Klink::Graphics
{
using TextureID = size_t;

class TextureManager
{
public:

	static void StaticInitialize(std::filesystem::path rootPath);
	static void StaticTerminate();
	static TextureManager* Get();

public:

	TextureManager() = default;
	~TextureManager();

	void Initialize();
	void Terminate();

	void SetRootPath(std::filesystem::path path);
	void UseRootPath(bool usesRootPath);

	TextureID LoadTexture(std::filesystem::path fileName);
	const Texture* GetTexture(TextureID textureId) const;

private:

	bool useRootPath = true;

	std::filesystem::path mRootPath = L"";
	std::unordered_map<TextureID, std::unique_ptr<Texture>> mInventory;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H