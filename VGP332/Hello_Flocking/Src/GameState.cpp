#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::AI;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });
	// Camera
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 1.0f });
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		KlinkApp::Shutdown();
		return;
	}

	const float moveSpeed = 15.0f;
	const float turnSpeed = 15.0f;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	SimpleDraw::DrawDebugGrid(100, { 0.4f, 0.4f, 0.4f, 1.0f });
}