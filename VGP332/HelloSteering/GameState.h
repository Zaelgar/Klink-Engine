#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

#include "Fish.h"
#include "Shark.h"


class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	// Debug and Controls
	float mSeekRange = 100.0f;

	Camera mCamera;
	AIWorld mAIWorld;

	Circle myOb { 350.0f, 250.0f, 100.0f };
	Circle myOb2{ 200.0f, 600.0f, 50.0f };
	Circle myOb3{ 900.0f, 200.0f, 75.0f };
	Circle myOb4{ 1100.0f, 600.0f, 25.0f };
	Circle myOb5{ 800.0f, 450.0f, 75.0f };

	LineSegment mTop{ 5.0f, 5.0f, 1275.0f, 5.0f };
	LineSegment mBottom{ 1275.0f, 715.0f, 5.0f, 715.0f};
	LineSegment mLeft{ 5.0f, 715.0f, 5.0f, 5.0f };
	LineSegment mRight{ 1275.0f, 5.0f, 1275.0f, 715.0f };

	//LineSegment mWall{ , };

	Fish* myFish;
	std::vector<Fish*> mFishes;

	Shark* myShark;
};
#endif // INCLUDED_GAMESTATE_H