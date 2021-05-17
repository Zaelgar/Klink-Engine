#ifndef INCLUDED_KLINK_AI_STEERINGBEHAVIOUR_H
#define INCLUDED_KLINK_AI_STEERINGBEHAVIOUR_H

#include "Common.h"

namespace Klink::AI
{

class Agent2D;

class SteeringBehaviour
{
public:

	SteeringBehaviour() = default;
	virtual ~SteeringBehaviour() = default;

	virtual Klink::JMath::Vector2 Calculate(Klink::AI::Agent2D& agent) = 0;
	//virtual const char * GetName() const = 0;

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }

	void SetPriority(int priority) { mPriority = priority; }
	const int GetPriority() { return mPriority; }

	float weight = 1.0f;

	bool operator<(const SteeringBehaviour& other) // HACK - instead of naming them with priority numbers, use actual priority
	{
		if (this->mPriority < other.mPriority)
			return true;
		else
			return false;
	}

private:

	bool mActive = false;
	int mPriority = 0;
};

} // namespace AI

#endif // INCLUDED_KLINK_AI_STEERINGBEHAVIOUR_H