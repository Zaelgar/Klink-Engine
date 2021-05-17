#include "Precompiled.h"

#include "CohesionBehaviour.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;

Vector2 CohesionBehaviour::Calculate(Agent2D& agent)
{
	Vector2 averageNeighbourPosition{};
	int count = 0;
	for (auto& fish : agent.world.GetNeighbourhood(agent.maxSpeed)) // TODO - AIWorld should check range once cells are added
	{
		float distance = Vector2::Distance(agent.position, fish->position);	// Distance check
		if (distance >= agent.maxSpeed)
			continue;

		averageNeighbourPosition += fish->position;
		count++;
	}

	averageNeighbourPosition /= static_cast<float>(count);

	return averageNeighbourPosition - agent.position;
}