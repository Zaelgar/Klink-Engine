#include "Precompiled.h"
#include "SamplerManager.h"

using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<SamplerManager> sInstance = nullptr;
}

void SamplerManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr,"[SamplerManager] System already initialized!");
	sInstance = std::make_unique<SamplerManager>();
	sInstance->Initialize();
}

void SamplerManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

SamplerManager* SamplerManager::Get()
{
	ASSERT(sInstance != nullptr, "[SamplerManager] No instance registered!");
	return sInstance.get();
}

void SamplerManager::Initialize()
{
	AddSampler("PointClamp", Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	AddSampler("PointWrap", Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	AddSampler("LinearClamp", Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	AddSampler("LinearWrap", Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	AddSampler("AnisoClamp", Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	AddSampler("AnisoWrap", Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void SamplerManager::Terminate()
{
	for (auto&[key, value] : mInventory)
		value->Terminate();
}

bool SamplerManager::AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sampler = std::make_unique<Sampler>();
		sampler->Initialize(filter, addressMode);
		iter->second = std::move(sampler);
	}
	return success;
}

Sampler* SamplerManager::GetSampler(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}