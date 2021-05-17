#ifndef INCLUDED_KLINK_AI_WALLAVOIDANCEBEHAVIOUR_H
#define INCLUDED_KLINK_AI_WALLAVOIDANCEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace Klink::AI
{

class WallAvoidanceBehaviour : public SteeringBehaviour
{
public:

	Klink::JMath::Vector2 Calculate(Agent2D& agent) override;

	float weight = 10.0f;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_WALLAVOIDANCEBEHAVIOUR_H