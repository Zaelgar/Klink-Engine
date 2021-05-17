#ifndef INCLUDED_KLINK_GRAPHICS_BLENDSTATEMANAGER_H
#define INCLUDED_KLINK_GRAPHICS_BLENDSTATEMANAGER_H

#include "BlendState.h"

namespace Klink::Graphics
{

class BlendStateManager
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static BlendStateManager* Get();

public:

	BlendStateManager() = default;

	void Initialize();
	void Terminate();

	bool AddBlendState(std::string name, BlendState::BlendMode blendMode);
	BlendState* GetSampler(std::string name);

private:

	std::unordered_map<std::string, std::unique_ptr<BlendState>> mInventory;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_TEXTUREMANAGER_H