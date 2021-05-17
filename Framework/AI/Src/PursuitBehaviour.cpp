#include "Precompiled.h"

#include "PursuitBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;

Vector2 PursuitBehaviour::Calculate(Agent2D& agent)
{
	// Get closest target of all neighbours
	Agent2D* target;
	float closestTargetDistance = 1000000.0f;
	for (auto& fish : agent.world.GetNeighbourhood(agent.pursuitRange))
	{
		float currentFishDistance = Vector2::Distance(fish->position, agent.position);
		if (currentFishDistance != 0 && currentFishDistance < closestTargetDistance)
		{
			closestTargetDistance = currentFishDistance;
			target = fish;
		}
	}

	// Calculate where it should be by the time you reach it
	Vector2 targetDestination = (target->position + target->velocity * (Vector2::Distance(agent.position, target->position) / agent.maxSpeed * 0.6f));


	// Set destination to that position
	agent.destination = targetDestination;

	// Seek to destination
	// Calculate desired velocity based off of position, destination, and multiply that normalized vector by max speed
	auto desiredVelocity = Vector2::Normalize(agent.destination + agent.velocity) * agent.maxSpeed;

	// desired velocity minus current velocity becomes the FORCE that we apply to get our agent to steer towards the destination velocity
	return desiredVelocity - agent.velocity;
}