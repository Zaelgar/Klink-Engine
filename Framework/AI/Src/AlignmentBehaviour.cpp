#include "Precompiled.h"

#include "AlignmentBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;
using namespace Klink::Graphics;

Vector2 AlignmentBehaviour::Calculate(Agent2D& agent)
{
	if (agent.isDebugging)
		SimpleDraw::DrawScreenCircle(agent.position, agent.maxSpeed);

	Vector2 averageNeighbourHeading{};
	for (auto& fish : agent.world.GetNeighbourhood(agent.maxSpeed)) // TODO - AIWorld should check range once cells are added
	{
		float distance = Vector2::Distance(agent.position, fish->position);	// Distance check
		if (distance >= agent.maxSpeed)
			continue;

		averageNeighbourHeading += fish->heading;
	}

	return averageNeighbourHeading.Normalized();
}