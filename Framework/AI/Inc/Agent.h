#ifndef INCLUDED_KLINK_AI_AGENT_H
#define INCLUDED_KLINK_AI_AGENT_H

#include "Common.h"

namespace Klink::AI
{

class AIWorld;
class Agent2D;

using AgentList = std::vector<Klink::AI::Agent2D*>;

class Agent2D
{
public:

	enum class AgentType { Fish, Shark, None };

	// REQUIRES AIWorld before an agent is constructed
	// Reference is not able to be nullptr
	Agent2D(AIWorld& world);
	~Agent2D();

	JMath::Matrix3 LocalToWorld();

	AIWorld& world;
	AgentList mNeighbours; // per frame, check once **IN AIWORLD**, use same neighbours for calcs
	AgentType agentType = AgentType::None;

	Klink::JMath::Vector2 position{};
	Klink::JMath::Vector2 velocity = (Klink::JMath::Vector2::RandomOnUnitCircle());
	Klink::JMath::Vector2 destination{};
	Klink::JMath::Vector2 heading = velocity.Normalized();
	bool isDebugging = false;


	float radius = 20.0f;
	float maxSpeed = 100.f;
	float mass = 1.0f;
	float maxSteeringForce = 4.0f;

	/// Behaviour Specific
	// Flee
	float pursuitRange = 200.0f;
	float fleeRange = 75.0f;

	// Wander
	float wanderDistance = 50.0f;
	float wanderRadius = 40.0f;
	float offsetScalar = 5.0f;

	Klink::JMath::Vector2 wanderPosition = (Klink::JMath::Vector2::RandomOnUnitCircle() * wanderRadius);

	// Obstacle Avoidance
};

class Agent3D
{
public:

	Agent3D(AIWorld& world);
	~Agent3D();

	JMath::Matrix4 LocalToWorld()
	{
		JMath::Vector3 l = { 0,0,1 };
		JMath::Vector3 u = heading;
		JMath::Vector3 r = Cross(u, l);
		JMath::Vector3 p = position;

		return
		{
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			l.x, l.y, l.z, 0.0f,
			p.x, p.y, p.z, 1.0f
		};
	}

	Klink::JMath::Vector3 position{};
	Klink::JMath::Vector3 velocity{};
	Klink::JMath::Vector3 destination{};
	Klink::JMath::Vector3 heading{};
	float radius = 1.0f;
	float maxSpeed = 100.f;
	float mass = 1.0f;
};
} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_AGENT_H

//TODO - Entity Class
// ENTITY CLASS is inherited by agent
// constains position and entity id static member
// Entity()
// :(entityID++)
//{}