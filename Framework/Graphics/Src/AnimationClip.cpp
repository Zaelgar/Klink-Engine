#include "Precompiled.h"
#include "AnimationClip.h"

using namespace Klink::Graphics;
using namespace Klink::JMath;

bool AnimationClip::GetTransform(float time, uint32_t boneIndex, Matrix4& transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->animation.GetTransform(time);
		return true;
	}
	return false;
}