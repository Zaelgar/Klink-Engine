#ifndef INCLUDED_KLINK_AI_EVADEBEHAVIOUR_H
#define INCLUDED_KLINK_AI_EVADEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{

class EvadeBehaviour : public SteeringBehaviour
{
public:

	Klink::JMath::Vector2 Calculate(Agent2D& agent) override;
	float weight = 0.01f;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_EVADEBEHAVIOUR_H