#include "Precompiled.h"
#include "Animation.h"

using namespace std;

using namespace Klink::Graphics;
using namespace Klink::JMath;

void Animation::Initialize(vector<Keyframe<Vector3>>& positionKeys, vector<Keyframe<Quaternion>>& rotationKeys, vector<Keyframe<Vector3>>& scaleKeys)
{
	mPositionKeyframes = std::move(positionKeys);
	mRotationKeyframes = std::move(rotationKeys);
	mScaleKeyframes = std::move(scaleKeys);
}

Matrix4 Animation::GetTransform(float time) const
{
	ASSERT(time >= 0, "[Animation] Get Transform argument (time) must be greater or equal to 0");

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	/// Position
	if (mPositionKeyframes.size() == 0)
	{
		position = Vector3::Zero();
	}
	else if (mPositionKeyframes.at(mPositionKeyframes.size() - 1).time < time) // Time is out of scope of animation
	{
		// Out of scope of animation - Setting to last keyframe
		position = mPositionKeyframes.at(mPositionKeyframes.size()-1).value;
	}
	else if (mPositionKeyframes.size() == 1)
	{
		position = mPositionKeyframes[0].value;
	}
	else
	{
		Keyframe<Vector3>* thisKey = nullptr;
		Keyframe<Vector3>* nextKey = nullptr;
		for (auto& keyframe : mPositionKeyframes)
		{
			if (time <= keyframe.time)
			{
				nextKey = new Keyframe<Vector3>(keyframe);
				break;
			}
			thisKey = new Keyframe<Vector3>(keyframe);
		}

		if (!thisKey)	// Means nextKey is actually the first key in the vector
		{
			position = nextKey->value;
		}
		else
		{
			/*
			float numerator = time - thisKey->time;
			float denominator = nextKey->time - thisKey->time;
			float t = numerator / denominator;
			*/

			position = Lerp(thisKey->value, nextKey->value, ((time-thisKey->time) / (nextKey->time - thisKey->time)) );
		}
	}

	/// Rotation
	if (mRotationKeyframes.size() == 0)
	{
		rotation = Quaternion::Identity();
	}
	else if (mRotationKeyframes.at(mRotationKeyframes.size() - 1).time < time)
	{
		// Out of scope of animation - Setting to last keyframe
		rotation = mRotationKeyframes.at(mRotationKeyframes.size() - 1).value;
	}
	else if (mRotationKeyframes.size() == 1)
	{
		rotation = mRotationKeyframes[0].value;
	}
	else
	{
		Keyframe<Quaternion>* thisKey = nullptr;
		Keyframe<Quaternion>* nextKey = nullptr;
		for (auto& keyframe : mRotationKeyframes)
		{
			if (time <= keyframe.time)
			{
				nextKey = new Keyframe<Quaternion>(keyframe);
				break;
			}
			thisKey = new Keyframe<Quaternion>(keyframe);
		}

		if (!thisKey)	// Means nextKey is actually the first key in the vector
		{
			rotation = nextKey->value;
		}
		else
		{
			/*
			float numerator = time - thisKey->time;
			float denominator = nextKey->time - thisKey->time;
			float t = numerator / denominator;
			*/

			rotation = Slerp(thisKey->value, nextKey->value, ((time - thisKey->time) / (nextKey->time - thisKey->time)));
		}
	}

	/// Scale
	if (mScaleKeyframes.size() == 0)
	{
		scale = Vector3::One();
	}
	else if (mScaleKeyframes.at(mScaleKeyframes.size() - 1).time < time)
	{
		// Out of scope of animation - Setting to last keyframe
		scale = mScaleKeyframes.at(mScaleKeyframes.size() - 1).value;
	}
	else if (mScaleKeyframes.size() == 1)
	{
		scale = mScaleKeyframes[0].value;
	}
	else
	{
		Keyframe<Vector3>* thisKey = nullptr;
		Keyframe<Vector3>* nextKey = nullptr;
		for (auto& keyframe : mScaleKeyframes)
		{
			if (time <= keyframe.time)
			{
				nextKey = new Keyframe<Vector3>(keyframe);
				break;
			}
			thisKey = new Keyframe<Vector3>(keyframe);
		}

		if (!thisKey)	// Means nextKey is actually the first key in the vector
		{
			scale = nextKey->value;
		}
		else
		{
			/*
			float numerator = time - thisKey->time;
			float denominator = nextKey->time - thisKey->time;
			float t = numerator / denominator;
			*/

			scale = Lerp(thisKey->value, nextKey->value, ((time - thisKey->time) / (nextKey->time - thisKey->time)));
		}
	}

	return CreateTransformMatrix(position, rotation, scale);
}