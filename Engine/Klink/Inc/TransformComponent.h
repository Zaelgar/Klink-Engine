#ifndef INCLUDED_KLINK_TRANSFORMCOMPONENT_H
#define INCLUDED_KLINK_TRANSFORMCOMPONENT_H

#include "Component.h"

namespace Klink
{
class TransformComponent : public Component
{
public:
	META_CLASS_DECLARE;

	JMath::Vector3 position;
	JMath::Quaternion rotation;
};
}

#endif // INCLUDED_KLINK_TRANSFORMCOMPONENT_H