#ifndef INCLUDED_KLINK_AI_AIWORLD_H
#define INCLUDED_KLINK_AI_AIWORLD_H

#include "Agent.h"

namespace Klink::AI
{

class AIWorld
{
public:

	using Obstacles = std::vector<Klink::JMath::Circle>;	// TODO JMath, add Circle and LineSegment structs
	using Walls = std::vector<Klink::JMath::LineSegment>;

	void AddObstacle(const Klink::JMath::Circle& obstacle);
	const Obstacles& GetObstacles() const;
	const Obstacles& GetObstaclesInNeighbourhood(const Agent2D& agent) const;

	void AddWall(const Klink::JMath::LineSegment& wall);
	const Walls& GetWalls() const { return mWalls; }

	void RegisterAgent(Agent2D* agent);
	void UnregisterAgent(Agent2D* agent);

	AgentList GetNeighbourhood(const JMath::Circle& range);
	AgentList GetNeighbourhood(const float range);

	void Update();

private:

	Obstacles mObstacles;
	Walls mWalls;
	AgentList mAgents;

};

}; // namespace Klink::AI

#endif	// INCLUDED_KLINK_AI_AIWORLD_H