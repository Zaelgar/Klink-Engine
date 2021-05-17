#ifndef INCLUDED_KLINK_GRAPHICS_ANIMATION_H
#define INCLUDED_KLINK_GRAPHICS_ANIMATION_H

#include "JMath/Inc/JMath.h"

#include "Keyframe.h"

namespace Klink::Graphics
{

class Animation
{
public:

	void Initialize(std::vector<Keyframe<Klink::JMath::Vector3>>& positionKeys,
		std::vector<Keyframe<Klink::JMath::Quaternion>>& rotationKeys,
		std::vector<Keyframe<Klink::JMath::Vector3>>& scaleKeys);

	JMath::Matrix4 GetTransform(float time) const;	// Decides which two frames to interpolate.
													// Watch for edge cases like only having a single frame or out of time boundary
													// LERP for position and scale, SLERP for rotation




	std::vector<Keyframe<Klink::JMath::Vector3>> mPositionKeyframes;
	std::vector<Keyframe<Klink::JMath::Quaternion>> mRotationKeyframes;
	std::vector<Keyframe<Klink::JMath::Vector3>> mScaleKeyframes;

};	// class Animation

class BoneAnimation
{
public:

	std::string boneName;
	Animation animation;
};

using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;

};	// namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_ANIMATION_H