#ifndef INCLUDED_KLINK_GRAPHICS_SKELETON_H
#define INCLUDED_KLINK_GRAPHICS_SKELETON_H

#include "Bone.h"

namespace Klink::Graphics
{

struct Skeleton
{
	Bone* root = nullptr;
	std::vector<std::unique_ptr<Bone>> bones;

	void Initialize(int boneCount);

	void AddBone();

	void Terminate();
};

} //  namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_SKELETON_H