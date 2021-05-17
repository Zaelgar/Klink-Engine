#include "Precompiled.h"

#include "WallAvoidanceBehaviour.h"

#include "Agent.h"
#include "AIWorld.h"

using namespace Klink::JMath;
using namespace Klink::AI;
using namespace Klink::Graphics;

Vector2 WallAvoidanceBehaviour::Calculate(Agent2D& agent)
{
	Matrix3 toLocalMatrix = Matrix3::Inverse(agent.LocalToWorld());
	Matrix3 toWorldMatrix = agent.LocalToWorld();

	LineSegment frontFeeler = { {0.0f, 0.0f}, {100.0f, 0.0f} };	
	LineSegment leftFeeler = {0.0f, frontFeeler.to * Matrix3::Rotation(45 * DegreesToRad) };	
	LineSegment rightFeeler = {0.0f, frontFeeler.to * Matrix3::Rotation(-45 * DegreesToRad) };
	
	frontFeeler.from = frontFeeler.from * toWorldMatrix;
	frontFeeler.to = frontFeeler.to * toWorldMatrix;

	leftFeeler.from = leftFeeler.from * toWorldMatrix;
	leftFeeler.to = leftFeeler.to * toWorldMatrix;

	rightFeeler.from = rightFeeler.from * toWorldMatrix;
	rightFeeler.to = rightFeeler.to * toWorldMatrix;

	if (agent.isDebugging)
	{
		SimpleDraw::DrawScreenLine(frontFeeler);
		SimpleDraw::DrawScreenLine(leftFeeler);
		SimpleDraw::DrawScreenLine(rightFeeler);
	}


	// Find closest Wall
	LineSegment wallToAvoid{};
	Vector2 thisWallIntersect;
	Vector2* closestWallIntersect = nullptr;
	float closestWallDistance = 10000000.0f;
	float thisWallDistance = closestWallDistance;

	for (auto wall : agent.world.GetWalls()) // TODO - GetObstaclesNeighbourhood() should only return entities within range
	{
		if (Intersect(frontFeeler, wall, thisWallDistance, thisWallIntersect))
		{
			if (thisWallDistance < closestWallDistance)
			{
				closestWallDistance = thisWallDistance;
				closestWallIntersect = &thisWallIntersect;
				wallToAvoid = wall;
			}
		}
		if (Intersect(leftFeeler, wall, thisWallDistance, thisWallIntersect))
		{
			if (thisWallDistance < closestWallDistance)
			{
				closestWallDistance = thisWallDistance;
				closestWallIntersect = &thisWallIntersect;
				wallToAvoid = wall;
			}
		}
		if (Intersect(rightFeeler, wall, thisWallDistance, thisWallIntersect))
		{
			if (closestWallDistance < closestWallDistance)
			{
				closestWallDistance = thisWallDistance;
				closestWallIntersect = &thisWallIntersect;
				wallToAvoid = wall;
			}
		}
	}

	if (closestWallIntersect)
	{
		float dX = wallToAvoid.to.x - wallToAvoid.from.x;
		float dY = wallToAvoid.to.y - wallToAvoid.from.y;

		Vector2 normal = { -dY, dX };

		Vector2 desiredVelocity = normal.Normalized() * ((agent.maxSpeed * 3.0f) - closestWallDistance);

		return desiredVelocity - agent.velocity;
	}

	return Vector2{};
};