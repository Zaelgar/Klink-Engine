#include "Precompiled.h"

#include "Component.h"

#include "GameObject.h"

using namespace Klink;
/*
META_CLASS_BEGIN(GameObject)
META_NO_FIELD
META_CLASS_END
*/

void GameObject::Initialize()
{
	for (auto& component : mComponents)
		component->Initialize();
}


void GameObject::Terminate()
{
	for (auto& component : mComponents)
		component->Terminate();
}


void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
		component->Update(deltaTime);
}


void GameObject::Render()
{
	for (auto& component : mComponents)
		component->Render();
}


void GameObject::DebugUI()
{
	for (auto& component : mComponents)
		component->DebugUI();
}

Component * Klink::GameObject::AddComponent(const Core::Meta::MetaClass * metaClass)
{
	Component* newComponent = static_cast<Component*>(metaClass->Create());
	mComponents.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}