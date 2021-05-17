#include "Precompiled.h"
#include "SteeringModule.h"

#include "Agent.h"

using namespace Klink::JMath;

Klink::AI::SteeringModule::SteeringModule(Agent2D& agent)
	: mAgent(agent)
{}

Vector2 Klink::AI::SteeringModule::Calculate()
{
	Vector2 total;

	if (mCalculationType == CalculationType::None)
	{
		for (auto&[name, behaviour] : mBehaviours)
		{
			if (behaviour->IsActive())
			{
				total += behaviour->Calculate(mAgent) * behaviour->weight;
			}
		}
	}
	else if (mCalculationType == CalculationType::Prioritized)
	{
		for (auto&[name, behaviour] : mBehaviours) // They are in order of priority
		{
			if (behaviour->IsActive())
			{
				total += behaviour->Calculate(mAgent);
				if (total.Magnitude() >= mAgent.maxSteeringForce)
					return total;
			}
		}
	}

	return total;
}