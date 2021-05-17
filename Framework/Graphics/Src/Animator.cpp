#include "Precompiled.h"
#include "Animator.h"

using namespace Klink::Graphics;
using namespace Klink::JMath;

namespace
{
	void UpdateTransforms(Bone* bone, std::vector<Matrix4>& boneMatrices)
	{
		if (bone->parent)
		{
			boneMatrices[bone->index] = bone->toParentTransform * boneMatrices[bone->parentIndex];
		}
		else
		{
			boneMatrices[bone->index] = bone->toParentTransform;
		}
		for (auto child : bone->children)
		{
			UpdateTransforms(child, boneMatrices);
		}
	}
}

void Animator::Initialize(Model* m)
{
	model = m;
	boneMatrices.resize(model->skeleton.bones.size(), Matrix4::Identity());
}

void Animator::Update(float deltaTime, bool isT)
{
	if (isT)	// means that deltaTime is the t value for LERP and SLERP in Animation
	{
		AnimationClip& animClip = *animationClip;

		for (unsigned int boneAnimIndex = 0; boneAnimIndex < animClip.boneAnimations.size(); ++boneAnimIndex)
		{
			if (animClip.boneAnimations[boneAnimIndex])
			{
				boneMatrices[boneAnimIndex] = animClip.boneAnimations[boneAnimIndex].get()->animation.GetTransform(deltaTime);
			}
			else
			{
				// already initialized to identity
				boneMatrices[boneAnimIndex] = Matrix4::Identity();
			}
		}

		UpdateTransforms(model->skeleton.root, boneMatrices);
	}
}

void Animator::PlayAnimation(std::string name, bool isLooping)
{

}