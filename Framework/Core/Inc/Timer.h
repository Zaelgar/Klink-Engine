#ifndef INCLUDED_CORE_TIMER_H
#define INCLUDED_CORE_TIMER_H

namespace Klink::Core
{

class Timer
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static Timer* Get();

public:

	bool Initialize();
	void Update();
	void Terminate();

	float GetDeltaTime();
	float GetFPS();
	float GetTimeSinceStart() { return mTimeSinceStart; }

	float mTimeSinceStart;

private:

	int mFrameCount = 0;
	unsigned long mFPSCounter = 0;
	float mFPS = 0.0f;
	float mTicksPerMs = 0.0f;
	float mDeltaTime = 0.0f;
	INT64 mStartTime = 0;
	INT64 mFrequency = 0;
};

} // namespace Klink::Core

#endif