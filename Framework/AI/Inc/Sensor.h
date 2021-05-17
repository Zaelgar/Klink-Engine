#ifndef INCLUDED_KLINK_AI_SENSOR_H
#define INCLUDED_KLINK_AI_SENSOR_H

#include "MemoryRecord.h"

namespace Klink::AI
{

	class Agent2D;

class Sensor
{
public:

	virtual ~Sensor() = default;

	virtual void Update(Agent2D& agent, MemoryRecords& memories, float deltaTime) = 0;
};

}; // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_SENSOR_H