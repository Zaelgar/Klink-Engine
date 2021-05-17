#ifndef INCLUDED_KLINK_AI_WANDERBEHAVIOUR_H
#define INCLUDED_KLINK_AI_WANDERBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{

class WanderBehaviour : public SteeringBehaviour
{
public:

	Klink::JMath::Vector2 Calculate(Agent2D& agent) override;

	float weight = 1.0f;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_WANDERBEHAVIOUR_H