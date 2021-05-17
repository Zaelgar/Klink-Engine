#ifndef INCLUDED_KLINK_GRAPHICS_ANIMATIONCLIP_H
#define INCLUDED_KLINK_GRAPHICS_ANIMATIONCLIP_H

#include "Animation.h"

namespace Klink::Graphics
{

class AnimationClip
{
public:

	bool GetTransform(float time, uint32_t boneIndex, Klink::JMath::Matrix4& transform) const;

	std::string name;
	float duration = 0.0f;
	float ticksPerSecond = 0.0f;

	BoneAnimations boneAnimations;
};

} // namespace Klink::Graphics;

#endif // INCLUDED_KLINK_GRAPHICS_ANIMATIONCLIP_H