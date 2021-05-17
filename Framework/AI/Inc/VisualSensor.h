#ifndef INCLUDED_KLINK_AI_VISUALSENSOR_H
#define INCLUDED_KLINK_AI_VISUALSENSOR_H

#include "Sensor.h"

namespace Klink::AI
{

class VisualSensor : public Sensor
{
public:

	void Update(Agent2D& agent, MemoryRecords& memory, float deltaTime) override;

	float viewRange = 0.0f;
	float viewAngle = 0.0f;
};

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_VISUALSENSOR_H