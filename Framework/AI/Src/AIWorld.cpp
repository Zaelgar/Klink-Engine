#include "Precompiled.h"
#include "AIWorld.h"

using namespace Klink::AI;

void AIWorld::AddObstacle(const Klink::JMath::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

const AIWorld::Obstacles & Klink::AI::AIWorld::GetObstacles() const
{
	return mObstacles;
}

const AIWorld::Obstacles & Klink::AI::AIWorld::GetObstaclesInNeighbourhood(const Agent2D& agent) const
{
	// TODO - Partition cell upgrade, search only adjacent
	return mObstacles;
}

void AIWorld::AddWall(const Klink::JMath::LineSegment& wall)
{
	mWalls.push_back(wall);
}

void AIWorld::RegisterAgent(Agent2D* agent)
{
	mAgents.push_back(agent);
}

void AIWorld::UnregisterAgent(Agent2D* agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
		std::iter_swap(iter, mAgents.end() - 1);
	
	mAgents.erase(iter);
}

AgentList AIWorld::GetNeighbourhood(const Klink::JMath::Circle& range)
{
	// TODO - update to use cell and grid system for optimized checks
	return mAgents;
}

AgentList AIWorld::GetNeighbourhood(const float range)
{
	// TODO - update to use cell and grid system for optimized checks
	return mAgents;
}

void Klink::AI::AIWorld::Update()
{

}