#include "Precompiled.h"

#include "SeekBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;
using namespace Klink::Graphics;

Vector2 SeekBehaviour::Calculate(Agent2D& agent)
{
	if (agent.isDebugging)
		SimpleDraw::DrawScreenCircle(agent.destination, 5.0f, Colours::RosyBrown);

	// Calculate desired velocity based off of position, destination, and multiply that normalized vector by max speed
	Vector2 desiredVelocity = Vector2::Normalize(agent.destination - agent.position) * agent.maxSpeed;

	// desired velocity minus current velocity becomes the FORCE that we apply to get our agent to steer towards the destination velocity
	return (desiredVelocity - agent.velocity);
}