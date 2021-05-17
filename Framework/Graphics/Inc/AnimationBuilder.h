#ifndef INCLUDED_KLINK_GRAPHICS_ANIMATIONBUILDER_H
#define INCLUDED_KLINK_GRAPHICS_ANIMATIONBUILDER_H

#include "JMath/Inc/JMath.h"

#include "Animation.h"

namespace Klink::Graphics
{

class AnimationBuilder	// BUILDER design pattern! - Google it!  https://en.wikipedia.org/wiki/Builder_pattern
{
public:

	AnimationBuilder& AddPositionKey(Klink::JMath::Vector3, float);
	AnimationBuilder& AddRotationKey(Klink::JMath::Quaternion, float);
	AnimationBuilder& AddScalingKey(Klink::JMath::Vector3, float);
	AnimationBuilder& AddKey(Klink::JMath::Vector3, Klink::JMath::Quaternion, Klink::JMath::Vector3, float);

	Animation Get();

private:

	std::vector<Keyframe<Klink::JMath::Vector3>> mPositionKeyframes;
	std::vector<Keyframe<Klink::JMath::Quaternion>> mRotationKeyframes;
	std::vector<Keyframe<Klink::JMath::Vector3>> mScaleKeyframes;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_ANIMATIONBUILDER_H