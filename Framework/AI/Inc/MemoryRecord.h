#ifndef INCLUDED_KLINK_AI_MEMORYRECORD_H
#define INCLUDED_KLINK_AI_MEMORYRECORD_H

#include "Common.h"

namespace Klink::AI
{

using Property = std::variant<int, float, Klink::JMath::Vector2>;

struct MemoryRecord
{
	std::unordered_map<std::string, Property> properties;

	float lastRecordedTime = 0.0f;
	float importance = 0.0f;

	// notes
	// int entityId = 0;
};

using MemoryRecords = std::list<MemoryRecord>;

} // namespace Klink::AI

#endif // INCLUDED_KLINK_AI_MEMORYRECORD_H