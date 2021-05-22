#include "Precompiled.h"
#include "ShaderManager.h"

using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<ShaderManager> sInstance = nullptr;
}

void ShaderManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr,"[ShaderManager] System already initialized!");
	sInstance = std::make_unique<ShaderManager>();
	sInstance->Initialize();
}

void ShaderManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

ShaderManager* ShaderManager::Get()
{
	ASSERT(sInstance != nullptr, "[ShaderManager] No instance registered!");
	return sInstance.get();
}

void ShaderManager::Initialize()
{
	/// Regular Shaders
	AddShader("StandardShader", Vertex::Format,  "../../Assets/Shaders/Standard.fx", "../../Assets/Shaders/Standard.fx");
	AddShader("TextureShader", VertexPX::Format, "../../Assets/Shaders/Texturing.fx", "../../Assets/Shaders/Texturing.fx");
	AddShader("TerrainShader", Vertex::Format, "../../Assets/Shaders/Terrain.fx", "../../Assets/Shaders/Terrain.fx");
	AddShader("TerrainTextureShader", Vertex::Format, "../../Assets/Shaders/TextureTerrain.fx", "../../Assets/Shaders/TextureTerrain.fx");

	/// Skybox
	AddShader("SkyboxShader", VertexPX::Format, "../../Assets/Shaders/Skybox.fx", "../../Assets/Shaders/Skybox.fx");

	/// Visual Helper Shaders
	AddShader("DepthMappingShader", Vertex::Format, "../../Assets/Shaders/DepthMapping.fx", "../../Assets/Shaders/DepthMapping.fx");

	/// Post Processing Shaders
	AddShader("BlurPostProcess", VertexPX::Format, "../../Assets/Shaders/Blur.fx", "../../Assets/Shaders/Blur.fx");
}

void ShaderManager::Terminate()
{
	for (auto&[key, value] : mInventory)
	{
		value->mPixelShader.Terminate();
		value->mVertexShader.Terminate();
	}
}

bool ShaderManager::AddShader(std::string name, uint32_t vertexFormat, std::filesystem::path vertexShaderPath, std::filesystem::path pixelShaderPath)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto shader = std::make_unique<Shader>();
		shader->mVertexShader.Initialize(vertexShaderPath.string(), vertexFormat);
		shader->mPixelShader.Initialize(pixelShaderPath.string());
		iter->second = std::move(shader);
	}
	return success;
}

Shader* ShaderManager::GetShader(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}