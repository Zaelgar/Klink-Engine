#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>
#include <imgui/Inc/AppLog.h>

class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	// All
	Klink::Graphics::Camera mCamera;

	AppLog mAppLog;

	Klink::ML::Dataset mDataset;
	Klink::ML::LinearRegression mLinReg;

	// LinReg settings
	int epoch = 1000;
	float learningRate = 0.0001f;

	Klink::ML::DatasetProperties mDatasetProperties;

	int sampleCount = 1000;
	/*
	float b0Value = 0.f;
	float b1Value = 0.f;
	float minX = 0.0f;
	float maxX = 100.0f;
	float noise = 10.0f;
	*/

	bool canRunAgain = false;
};
#endif // INCLUDED_GAMESTATE_H