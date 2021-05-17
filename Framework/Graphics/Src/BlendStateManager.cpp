#include "Precompiled.h"
#include "BlendStateManager.h"

using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<BlendStateManager> sInstance = nullptr;
}

void BlendStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr,"[SamplerManager] System already initialized!");
	sInstance = std::make_unique<BlendStateManager>();
	sInstance->Initialize();
}

void BlendStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

BlendStateManager* BlendStateManager::Get()
{
	ASSERT(sInstance != nullptr, "[SamplerManager] No instance registered!");
	return sInstance.get();
}

void BlendStateManager::Initialize()
{
	AddBlendState("Opaque", BlendState::BlendMode::Opaque);
	AddBlendState("AlphaBlend", BlendState::BlendMode::AlphaBlend);
	AddBlendState("AlphaPremultiplied", BlendState::BlendMode::AlphaPremulitplied);
	AddBlendState("Additive", BlendState::BlendMode::Additive);
}

void BlendStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool BlendStateManager::AddBlendState(std::string name, BlendState::BlendMode blendMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto blendState = std::make_unique<BlendState>();
		blendState->Initialize(blendMode);
		iter->second = std::move(blendState);
	}
	return success;
}

BlendState* BlendStateManager::GetSampler(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}