#ifndef INCLUDED_KLINK_GRAPHICS_SAMPLERMANAGER_H
#define INCLUDED_KLINK_GRAPHICS_SAMPLERMANAGER_H

#include "Sampler.h"

namespace Klink::Graphics
{

class SamplerManager
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static SamplerManager* Get();

public:

	SamplerManager() = default;
	//~SamplerManager();

	void Initialize();
	void Terminate();

	bool AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode);
	Sampler* GetSampler(std::string name);

private:

	std::unordered_map<std::string, std::unique_ptr<Sampler>> mInventory;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H