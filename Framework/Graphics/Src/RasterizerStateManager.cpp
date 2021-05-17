#include "Precompiled.h"
#include "RasterizerStateManager.h"

using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<RasterizerStateManager> sInstance = nullptr;
}

void RasterizerStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr,"[RasterizerManager] System already initialized!");
	sInstance = std::make_unique<RasterizerStateManager>();
	sInstance->Initialize();
}

void RasterizerStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

RasterizerStateManager* RasterizerStateManager::Get()
{
	ASSERT(sInstance != nullptr, "[RasterizerManager] No instance registered!");
	return sInstance.get();
}

void RasterizerStateManager::Initialize()
{
	AddRasterizerState("Solid", RasterizerState::CullMode::Back, RasterizerState::FillMode::Solid);
	AddRasterizerState("Wireframe", RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullFrontSolid", RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullFrontWireframe", RasterizerState::CullMode::Front, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullNoneSolid", RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullNoneWireframe", RasterizerState::CullMode::None, RasterizerState::FillMode::Wireframe);
}

void RasterizerStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool RasterizerStateManager::AddRasterizerState(std::string name, RasterizerState::CullMode cullMode, RasterizerState::FillMode fillMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto rasterState = std::make_unique<RasterizerState>();
		rasterState->Initialize(cullMode, fillMode);
		iter->second = std::move(rasterState);
	}
	return success;
}

RasterizerState* RasterizerStateManager::GetRasterizerState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}