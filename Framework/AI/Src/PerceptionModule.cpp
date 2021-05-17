#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace Klink::AI;
using namespace Klink::Core;

PerceptionModule::PerceptionModule(Agent2D& agent, ImportanceCalculator importanceCalculator)
	:mAgent(agent)
	, mImportanceCalculator(std::move(importanceCalculator))
{

}

PerceptionModule::~PerceptionModule()
{}

void PerceptionModule::Update(float deltaTime)
{
	for (auto&[name, sensor] : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltaTime);
	}

	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		if (iter->lastRecordedTime + mMemorySpan < Timer::Get()->GetTimeSinceStart())
			iter = mMemory.erase(iter);
		else
			++iter;
	}

	for (auto m : mMemory)
	{
		mImportanceCalculator(m);
	}

	mMemory.sort([](MemoryRecord& a, MemoryRecord& b)
	{
		return a.importance > b.importance;
		});

	// TODO - memory
	// remove any records older than mMemorySpan
}