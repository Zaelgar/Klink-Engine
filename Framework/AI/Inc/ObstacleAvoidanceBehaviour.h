#ifndef INCLUDED_KLINK_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H
#define INCLUDED_KLINK_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{

class ObstacleAvoidanceBehaviour : public SteeringBehaviour
{
public:

	Klink::JMath::Vector2 Calculate(Agent2D& agent) override;

private:

	Klink::JMath::Rectangle CreateBoundingBox(Klink::AI::Agent2D& agent);

	bool Intersect(Klink::JMath::Circle obstacleToCheck, Agent2D& agent, Klink::JMath::Vector2* intersectingPoint);

	float weight = 10.0f;
private:

	int priority;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H