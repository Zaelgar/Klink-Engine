#ifndef INCLUDED_KLINK_GRAPHICS_BONE_H
#define INCLUDED_KLINK_GRAPHICS_BONE_H

#include "Common.h"

namespace Klink::Graphics
{

struct Bone
{
	std::string name;
	int index;

	Bone* parent = nullptr;
	int parentIndex = -1;

	std::vector<Bone*> children;
	std::vector<int> childIndices;

	Klink::JMath::Matrix4 toParentTransform;
	Klink::JMath::Matrix4 offsetTransform;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_BONE_H