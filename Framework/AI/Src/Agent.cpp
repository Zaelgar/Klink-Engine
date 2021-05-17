#include "Precompiled.h"
#include "Agent.h"

#include "AIWorld.h"

using namespace Klink::AI;
using namespace Klink::JMath;

Agent2D::Agent2D(AIWorld& _world)
	:world(_world)
{
	world.RegisterAgent(this);
}

Agent2D::~Agent2D()
{
	world.UnregisterAgent(this);
}

Matrix3 Agent2D::LocalToWorld()
{
	Vector2 x = heading;
	Vector2 y = { heading.y, -heading.x };
	Vector2 p = position;
	
	return Matrix3
	{
		x.x, x.y, 0.0f,
		y.x, y.y, 0.0f,
		p.x, p.y, 1.0f
	};
}