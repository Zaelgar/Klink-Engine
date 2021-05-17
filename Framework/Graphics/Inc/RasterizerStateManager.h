#ifndef INCLUDED_KLINK_GRAPHICS_RASTERIZERSTATEMANAGER_H
#define INCLUDED_KLINK_GRAPHICS_RASTERIZERSTATEMANAGER_H

#include "RasterizerState.h"

namespace Klink::Graphics
{

class RasterizerStateManager
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static RasterizerStateManager* Get();

public:

	RasterizerStateManager() = default;

	void Initialize();
	void Terminate();

	bool AddRasterizerState(std::string name, RasterizerState::CullMode cullMode, RasterizerState::FillMode fillMode);
	RasterizerState* GetRasterizerState(std::string name);

private:

	std::unordered_map<std::string, std::unique_ptr<RasterizerState>> mInventory;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_RASTERIZERSTATEMANAGER_H