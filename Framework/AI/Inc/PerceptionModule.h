#ifndef INCLUDED_KLINK_AI_PERCEPTIONMODULE_H
#define INCLUDED_KLINK_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace Klink::AI
{

class Agent2D;

using ImportanceCalculator = std::function<void(MemoryRecord&)>;

class PerceptionModule
{
public:
	PerceptionModule(Agent2D& agent, ImportanceCalculator importanceCalculator);
	~PerceptionModule();

	template <class SensorType>
	SensorType* AddSensor(std::string name);

	template <class SensorType>
	SensorType* GetSensor(const std::string& name);

	void Update(float deltaTime);

	const MemoryRecords& GetMemoryRecords() const { return mMemory; }

private:

	using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

	Agent2D& mAgent;
	SensorMap mSensors;
	ImportanceCalculator mImportanceCalculator;

	MemoryRecords mMemory;
	// notes
	 float mMemorySpan = 15.0f; //Can remember 15 secs of history
};

template <class SensorType>
SensorType* PerceptionModule::AddSensor(std::string name)
{
	auto[iter, success] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
	return success ? static_cast<SensorType*>(iter->second.get()) : nullptr;
}

template <class SensorType>
SensorType* PerceptionModule::GetSensor(const std::string& name)
{
	auto iter = mSensors.find(name);
	return iter != mSensors.end() ? static_cast<SensorType*>(iter->second.get()) : nullptr;
}


}
#endif // INCLUDED_KLINK_AI_PERCEPTIONMODULE_H