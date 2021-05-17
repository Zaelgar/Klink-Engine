#ifndef INCLUDED_KLINK_COLLIDERCOMPONENT_H
#define INCLUDED_KLINK_COLLIDERCOMPONENT_H

#include "Component.h"

namespace Klink
{
	class TransformComponent;

class ColliderComponent : public Component
{
public:
	META_CLASS_DECLARE;

	void Initialize() override;
	void Render() override;

	const Klink::TransformComponent* mTransformComponent = nullptr;
	JMath::Vector3 center;
	JMath::Vector3 extend;
};
}

#endif // INCLUDED_KLINK_COLLIDERCOMPONENT_H