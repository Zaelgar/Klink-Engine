#include "Precompiled.h"
#include "VisualSensor.h"

using namespace Klink::AI;

void VisualSensor::Update(Agent2D& agent, MemoryRecords& memory, float deltaTime)
{
	/*
	for (auto& mineral : minerals)
		{
		// check distance
		// check view angle
		// check LOS (raycast)
		// Always look for early outs

		auto viewRangeSqr = (viewRange * viewRange); // cache
		auto cosViewAngle = cos(viewAngle * 0.5f); // cache

		if (/*TODO - DistanceSqr for JMath(Klink::JMath::Distance(agent.position, mineral) * Klink::JMath::Distance(agent.position, mineral)) > viewRangeSqr/*TODO - JMath Square function)
			continue;

		if (Klink::JMath::Dot(agent.heading, Klink::JMath::Normalize(mineral - agent.position)))
			continue;

		find in memory
		if(found)
			 update record
		else
			new record
		
	}
	*/
}