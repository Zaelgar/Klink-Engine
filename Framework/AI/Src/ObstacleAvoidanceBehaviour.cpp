#include "Precompiled.h"

#include "ObstacleAvoidanceBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;
using namespace Klink::Graphics;

Vector2 ObstacleAvoidanceBehaviour::Calculate(Agent2D& agent)
{
	Matrix3 localToWorld = agent.LocalToWorld();

	if (agent.isDebugging)
	{
		Klink::JMath::Rectangle boundingBox = CreateBoundingBox(agent);
	}

	// Gather all potential Obstacles (eliminating ones behind)
	Circle obstacleToAvoid{};
	Vector2 obstacleLocalPos{};
	float closestObstacleDistance = 10000000.0f;

	for (auto obstacle : agent.world.GetObstaclesInNeighbourhood(agent)) // TODO - GetObstaclesNeighbourhood() should only return entities within range
	{
		obstacleLocalPos = obstacle.center * Matrix3::Inverse( agent.LocalToWorld() );
		float obstacleDistance = Vector2::Distance(obstacleLocalPos, Vector2::Zero());
		if (obstacleLocalPos.x <= 0.0f)	// Obstacle is behind entity
		{
			continue;
		}
		if (obstacleDistance >= agent.maxSpeed + obstacle.radius)	// Obstacle is too far away
		{
			continue;
		}
		if (abs(obstacleLocalPos.y) > obstacle.radius + agent.radius) // Obstacle is too far left or right
		{
			continue;
		}
		if (obstacleDistance < closestObstacleDistance)
		{
			closestObstacleDistance = obstacleDistance;
		}

		obstacleToAvoid = obstacle;
	}
	obstacleLocalPos = obstacleToAvoid.center * Matrix3::Inverse(agent.LocalToWorld());
	if (obstacleToAvoid.radius == 0.0f)
	{
		return Vector2{};
	}

	if (agent.isDebugging)
	{
		LineSegment forward = { Vector2::Zero(), Vector2::XAxis() * agent.maxSpeed };
		SimpleDraw::DrawScreenLine(forward.from, forward.to);
	}

	Vector2 intersectPoint{};
	if(Intersect(obstacleToAvoid, agent, &intersectPoint))
	{
		float length = (agent.radius * 2.0f) + (agent.velocity.Magnitude() / agent.maxSpeed) * agent.radius * 2.0f;
		float weight = 1.0f + (length - obstacleLocalPos.x) / length;

		float forceY = (obstacleToAvoid.radius - obstacleLocalPos.y) * weight;
		
		float brakeWeight = 0.2f;
		float forceX = (obstacleToAvoid.radius - obstacleLocalPos.x) * brakeWeight;

		Vector2 forceApplied{ forceX, forceY };

		forceApplied = forceApplied * agent.LocalToWorld();

		return forceApplied;
	}
	return Vector2{};
}

Klink::JMath::Rectangle Klink::AI::ObstacleAvoidanceBehaviour::CreateBoundingBox(Agent2D& agent)
{
	float length = (agent.radius * 2.0f) + (agent.velocity.Magnitude() / agent.maxSpeed) * agent.radius * 2.0f;
	float width = agent.radius / 2.0f;
	Klink::JMath::Rectangle boundingBox =
	{
		{0.0f, width},
		{length, width},
		{length, -width},
		{0.0f, -width}
	};
	boundingBox.topLeft = Matrix3::TransformCoord(boundingBox.topLeft, agent.LocalToWorld());
	boundingBox.topRight = Matrix3::TransformCoord(boundingBox.topRight, agent.LocalToWorld());
	boundingBox.bottomLeft = Matrix3::TransformCoord(boundingBox.bottomLeft, agent.LocalToWorld());
	boundingBox.bottomRight = Matrix3::TransformCoord(boundingBox.bottomRight, agent.LocalToWorld());
	SimpleDraw::DrawScreenRect(boundingBox.topLeft, boundingBox.topRight, boundingBox.bottomLeft, boundingBox.bottomRight);

	return boundingBox;
}

bool ObstacleAvoidanceBehaviour::Intersect(Circle obstacleToCheck, Agent2D& agent, Vector2* intersectingPoint)
{
	//x = cX + / -sqrt(r ^ 2 - cY ^ 2) for y = 0

	Vector2 localObstaclePosition = obstacleToCheck.center * Matrix3::Inverse(agent.LocalToWorld());
	float expandedRadius = agent.radius + obstacleToCheck.radius;
	float sR = sqrt(expandedRadius*expandedRadius - localObstaclePosition.y*localObstaclePosition.y);

	float intersectX = localObstaclePosition.x - sR;
	if (sR <= 0.0f)
	{
		intersectX = localObstaclePosition.x + sR;
	}

	Vector2 iP = { intersectX, 0.0f };
	iP = iP * agent.LocalToWorld();
	*intersectingPoint = iP;
	
	if(agent.isDebugging)
		SimpleDraw::DrawScreenCircle(iP, 5.0f, Colours::Brown);

	return true;
}