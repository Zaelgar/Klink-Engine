#include "Precompiled.h"
#include "Timer.h"

#include "Debug.h"

using namespace Klink::Core;

namespace
{
	std::unique_ptr<Timer> sInstance = nullptr;
}

void Timer::StaticInitialize()
{
	ASSERT(sInstance == nullptr,"[Timer] System already initialized!");
	sInstance = std::make_unique<Timer>();
	sInstance->Initialize();
}

void Timer::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

Timer* Timer::Get()
{
	ASSERT(sInstance != nullptr, "[Timer] No instance registered!");
	return sInstance.get();
}

bool Timer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&mFrequency);
	if (mFrequency == 0)
	{
		return false;
	}

	mTicksPerMs = static_cast<float>(mFrequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);

	mTimeSinceStart = 0.0f;

	return true;
}

void Timer::Update()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = static_cast<float>(currentTime - mStartTime);

	mDeltaTime = timeDifference / mTicksPerMs;
	mDeltaTime /= 1000.0f;

	mFrameCount++;
	mFPSCounter += mDeltaTime;
	if (mFPSCounter >= 1000)
	{ 
		mFPSCounter = 0;
		mFPS = mFrameCount;
		mFrameCount = 0;
	}

	mStartTime = currentTime;
	mTimeSinceStart += mDeltaTime;

	return;
}

void Timer::Terminate()
{

}

float Timer::GetDeltaTime()
{
	return mDeltaTime;
}

float Timer::GetFPS()
{
	return mFPS;
}