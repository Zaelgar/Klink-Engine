#include "Precompiled.h"

#include "WanderBehaviour.h"

#include "Agent.h"

using namespace Klink::JMath;
using namespace Klink::AI;
using namespace Klink::Graphics;

Vector2 WanderBehaviour::Calculate(Agent2D& agent)
{
	if (agent.isDebugging)
		SimpleDraw::DrawScreenCircle(agent.destination, 5.0f, Colours::RosyBrown);

	Vector2 circlePosition = (agent.heading * agent.wanderDistance) + agent.position;

	agent.wanderPosition += Vector2::RandomOnUnitCircle() * agent.offsetScalar;
	agent.wanderPosition.Normalize();
	agent.wanderPosition *= agent.wanderRadius;

	agent.destination = agent.wanderPosition + circlePosition;

	// Calculate desired velocity based off of position, destination, and multiply that normalized vector by max speed
	Vector2 desiredVelocity = Vector2::Normalize(agent.destination - agent.position) * agent.maxSpeed;

	// desired velocity minus current velocity becomes the FORCE that we apply to get our agent to steer towards the destination velocity
	return (desiredVelocity - agent.velocity);
}