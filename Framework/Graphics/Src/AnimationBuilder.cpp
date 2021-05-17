#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace Klink;
using namespace Graphics;
using namespace JMath;

// return *this;	// Called Method Chaining::https://en.wikipedia.org/wiki/Method_chaining
AnimationBuilder& AnimationBuilder::AddPositionKey(Vector3 v, float t)
{
	mPositionKeyframes.emplace_back(v, t);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(Quaternion q, float t)
{
	mRotationKeyframes.emplace_back(q, t);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScalingKey(Vector3 v, float t)
{
	mScaleKeyframes.emplace_back(v, t);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddKey(Vector3 translation, Quaternion rotation, Vector3 scale, float t)
{
	mPositionKeyframes.emplace_back(translation, t);
	mRotationKeyframes.emplace_back(rotation, t);
	mScaleKeyframes.emplace_back(scale, t);
	return *this;
}

Animation AnimationBuilder::Get()
{
	Animation anim;
	anim.Initialize(mPositionKeyframes, mRotationKeyframes, mScaleKeyframes);

	return anim;
}