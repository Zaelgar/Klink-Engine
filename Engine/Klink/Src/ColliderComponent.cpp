#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace Klink;

/*
META_DERIVED_BEGIN(Klink::ColliderComponent, Klink::Component)
	META_FIELD_BEGIN
		META_FIELD(center, "Center")
		META_FIELD(extend, "Extend")
	META_FIELD_END
META_CLASS_END
*/

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void ColliderComponent::Render()
{
	//Graphics::SimpleDraw::AddAABB(center + mTransformComponent->position, extend, Graphics::Colors::Gray, true); // true and false is filled or wireframe
	//Graphics::SimpleDraw::AddAABB(center + mTransformComponent->position, extend, Graphics::Colors::Cyan, false);
}