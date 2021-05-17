#ifndef INCLUDED_KLINK_GRAPHICS_SHADERMANAGER_H
#define INCLUDED_KLINK_GRAPHICS_SHADERMANAGER_H

#include "VertexTypes.h"

#include "VertexShader.h"
#include "PixelShader.h"

struct Shader
{
	Klink::Graphics::VertexShader mVertexShader;
	Klink::Graphics::PixelShader mPixelShader;

	void Bind()
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
	}
};

namespace Klink::Graphics
{

class ShaderManager
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static ShaderManager* Get();

public:

	ShaderManager() = default;

	void Initialize();
	void Terminate();

	bool AddShader(std::string name, uint32_t vertexFormat, std::filesystem::path vertexShaderPath, std::filesystem::path pixelShaderPath);
	Shader* GetShader(std::string name);

private:

	std::unordered_map<std::string, std::unique_ptr<Shader>> mInventory;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H