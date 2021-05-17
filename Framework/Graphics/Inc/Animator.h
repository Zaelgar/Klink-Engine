#ifndef INCLUDED_KLINK_GRAPHICS_ANIMATOR_H
#define INCLUDED_KLINK_GRAPHICS_ANIMATOR_H

#include "Animation.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include "Model.h"
#include "Bone.h"

namespace Klink::Graphics
{

class Animator
{
public:

	void Initialize(Klink::Graphics::Model* model);

	std::vector<Klink::JMath::Matrix4>& GetBoneMatrices() { return boneMatrices; }

	void Update(float deltaTime, bool isT = false);
	void PlayAnimation(std::string name, bool isLooping = false);

	Klink::Graphics::Model* model;

	std::vector<Klink::JMath::Matrix4> boneMatrices;

	AnimationClip* animationClip;
};

}	// namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_ANIMATOR_H