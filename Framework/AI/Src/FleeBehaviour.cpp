#include "Precompiled.h"

#include "FleeBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;

Vector2 FleeBehaviour::Calculate(Agent2D& agent)
{
	// Get closest target of all neighbours
	Agent2D* targetToAvoid = nullptr;
	float closestTargetDistance = 1000000.0f;
	for (auto& shark : agent.world.GetNeighbourhood(agent.fleeRange))
	{
		if (shark->agentType == Agent2D::AgentType::Shark)	// not a shark, don't need to avoid
		{
			float currentFishDistance = Vector2::Distance(shark->position, agent.position);
			if (currentFishDistance != 0 && currentFishDistance < closestTargetDistance)
			{
				closestTargetDistance = currentFishDistance;
				targetToAvoid = shark;
			}
		}
	}

	if (!targetToAvoid || Vector2::DistanceSquared(targetToAvoid->position, agent.position) >= (agent.fleeRange * agent.fleeRange))
	{
		return Vector2{};
	}

	// Calculate desired velocity based off of position, destination, and multiply that normalized vector by max speed
	auto desiredVelocity = Vector2::Normalize(agent.position - targetToAvoid->position) * agent.maxSpeed;

	// desired velocity minus current velocity becomes the FORCE that we apply to get our agent to steer towards the destination velocity
	return desiredVelocity - agent.velocity;
}