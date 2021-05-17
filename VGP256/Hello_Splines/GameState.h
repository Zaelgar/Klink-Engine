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

	// general
	Klink::Graphics::Camera mCamera;

	bool isDrawingQuadratic = false;
	bool isDrawingCubic = false;
	bool isDrawingCatmullRom = false;

	float timeCount = 0.0f;
	float totalTravelTime = 3.0f;

	// Positions
	std::vector<Vector2> mPositions;

	Vector2 qPos1 = { 50.0f, 50.0f };
	Vector2 qPos2 = { 400.0f, 300.0f };
	Vector2 qPos3 = { 800.0f, 20.0f };

	Vector2 cPos1 = { 50.0f, 100.0f };
	Vector2 cPos2 = { 400.0f, 350.0f };
	Vector2 cPos3 = { 600.0f, 100.0f };
	Vector2 cPos4 = { 800.0f, 350.0f };

	Vector2 catmulPos1 = { 50.0f, 390.0f };
	Vector2 catmulPos2 = { 150.0f, 450.0f };
	Vector2 catmulPos3 = { 250.0f, 550.0f };
	Vector2 catmulPos4 = { 550.0f, 400.0f };
	Vector2 catmulPos5 = { 650.0f, 700.0f };
	Vector2 catmulPos6 = { 800.0f, 650.0f };
};
#endif // INCLUDED_GAMESTATE_H