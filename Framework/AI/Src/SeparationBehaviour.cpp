#include "Precompiled.h"

#include "SeparationBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;

Vector2 SeparationBehaviour::Calculate(Agent2D& agent)
{
	Vector2 accumulatedSeparationForce{};
	for (auto& fish : agent.world.GetNeighbourhood(agent.maxSpeed)) // TODO - AIWorld should check range once cells are added
	{
		float distance = Vector2::Distance(agent.position, fish->position);
		if (distance >= agent.maxSpeed || fish->position == agent.position)
			continue;

		Vector2 neighbourToAgent = ((agent.position - fish->position).Normalized() /= distance);	// normalized vector from neighbour to agent, divided by distance

		accumulatedSeparationForce += neighbourToAgent;
	}


	return accumulatedSeparationForce * 100.0f;
}