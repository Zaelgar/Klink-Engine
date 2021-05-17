#ifndef INCLUDED_KLINK_AI_PURSUITBEHAVIOUR_H
#define INCLUDED_KLINK_AI_PURSUITBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{

class PursuitBehaviour : public SteeringBehaviour
{
public:

	Klink::JMath::Vector2 Calculate(Agent2D& agent) override;
	float weight = 1.0f;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_PURSUITBEHAVIOUR_H