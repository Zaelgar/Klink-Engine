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

	int mPopulationCount = 100;
	float mCrossOverRate = 0.45f;
	float mMutationRate = 0.1f;

	Klink::ML::GeneticAlgorithm mGeneticAlgorithm;

	AppLog mAppLog;
	bool mIsInitialized = false;
};

#endif // #ifndef INCLUDED_GAMESTATE_H
