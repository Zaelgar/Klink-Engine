#include "Precompiled.h"

#include "ArriveBehaviour.h"
#include "Agent.h"
#include "AIWorld.h"


using namespace Klink::JMath;
using namespace Klink::AI;

Vector2 ArriveBehaviour::Calculate(Agent2D& agent)
{
	Vector2 toTarget = agent.destination - agent.position;
	float distance = toTarget.Magnitude();

	if (distance > 0.0f)
	{
		const float decelTweaker = 0.3f;
		float speed = distance / (decelTweaker * 5.0f);

		speed = std::min(speed, agent.maxSpeed);

		Vector2 desiredVelocity = toTarget * speed / distance;

		return (desiredVelocity - agent.velocity);
	}

	return Vector2{ 0.0f, 0.0f };
}