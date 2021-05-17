#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>
#include <ImGui/Inc/AppLog.h>

class GameState : public Klink::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void GenerateData();
	void Train();

	AppLog mAppLog;

	Klink::ML::Dataset mDataset;
	Klink::ML::LogisticRegression mLogReg;

	int mSampleCount = 100;
	int mIteration = 100;

	float mTargetB0 = 0.0f;
	float mTargetB1 = 1.0f;
	float mTargetB2 = 1.0f;
	float mMinX = 0.0f;
	float mMaxX = 100.0f;
	float mNoise = 10.0f;
};

#endif // #ifndef INCLUDED_GAMESTATE_H
