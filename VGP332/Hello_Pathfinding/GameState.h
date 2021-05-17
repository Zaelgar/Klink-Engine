#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

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


	// Pathfinding Vars
	bool isFinding = false;
	bool needsRebuild = true;
	bool redrawReset = false;

	bool bindNeighboursClockwise = true;
	int columns = 23;
	int rows = 13;
	int size = 0;
	float radius = 20.0f;
	int cellPadding = 15;
	int marginPadding = 15;

	int startX = 11, startY = 4;
	int endX = 16, endY = 5;

	Klink::AI::NavGraph graph;

	GraphSearchContext context;

	std::list<size_t> path;

	Timer loopTimer;

	bool done = false;
	// Last time, on upgrading AI...
	// Converting old format (WinMain Copy.cpp) to new format (Winmain.cpp)
	// Ended on line 102 I think ^
};
#endif // INCLUDED_GAMESTATE_H