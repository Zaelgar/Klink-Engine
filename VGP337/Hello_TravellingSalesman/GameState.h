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

	Klink::Graphics::Camera mCamera;

	Klink::ML::GeneticAlgorithm mGeneticAlgorithm;

	int mPopulation = 100;
	float mCrossOverRate = 0.45f;
	float mMutationRate = 0.1f;

	std::vector<Vector2> mLocations;
	int mNumLocations = 10;
	int mIterations = 100;
	int mCurrentIteration = 0;
	bool mIsUsingIterations = false;
	float mFitnessThreshold = 10000.0f;

	bool mIsFinished = false;
	float mCityRadius = 20.0f;
	float minX = 450.0f;
	float minY = 50.0f;
	float maxX = 1230.0f;
	float maxY = 670.0f;
	Klink::Graphics::Colours::Colour mCityColour = Colours::AliceBlue;

	float mCircleRadius = 250.0f;
	Vector2 mCircleCenter = {800.0f,380.0f};

	AppLog mAppLog;
	bool mIsInitialized = false;
};

#endif // #ifndef INCLUDED_GAMESTATE_H