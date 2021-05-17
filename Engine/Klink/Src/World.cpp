#include "Precompiled.h"
#include "World.h"

using namespace Klink;

void World::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "[World] World is already initialized");

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);

	mInitialized = true;
}

void World::Terminate()
{
	if (!mInitialized)
		return;

	for (auto& service : mServices)
		service->Terminate();

	mGameObjectFactory.reset();
	mGameObjectHandlePool.reset();
	mGameObjectFactory.reset();

	mInitialized = false;
}

void World::Update(float deltaTime)
{
	for (auto gameObject : mUpdateList)
		gameObject->Update(deltaTime);

	for (auto& service : mServices)
		service->Update(deltaTime);
}

void World::Render()
{
	for (auto gameObject : mUpdateList)
		gameObject->Render();

	for (auto& service : mServices)
		service->Render();
}

void World::DebugUI()
{
	for (auto gameObject : mUpdateList)
		gameObject->DebugUI();

	for (auto& service : mServices)
		service->DebugUI();
}