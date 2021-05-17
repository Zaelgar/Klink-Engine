#include "HullGenerationState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;

void HullGenerationState::Initialize()
{
	RandomPointsInRectangle(mPoints, areaBoundary, pointCount);
}

void HullGenerationState::Terminate()
{
	mPoints.clear();
}

void HullGenerationState::Update(float deltaTime)
{
	/// Input for 3D
	{
		auto inputSystem = InputSystem::Get();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			KlinkApp::Shutdown();
			return;
		}

		float moveSpeed = 15.0f;
		const float turnSpeed = 15.0f;
		if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
			moveSpeed = 30.0f;
		else
			moveSpeed = 15.0f;
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
}

void HullGenerationState::Render()
{
	SimpleDraw::DrawScreenRect(areaBoundary.min, areaBoundary.max, Colours::OrangeRed);
	
	for (auto p : mPoints)
	{
		SimpleDraw::DrawScreenCircle(p, 5.0f, Colours::MediumPurple);
	}

	SimpleDraw::DrawScreenDebugGrid(50, Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	SimpleDraw::Render(mCamera);
}

void HullGenerationState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Change State"))
	{
		KlinkApp::ChangeState("GameState");
	}
	
	ImGui::End();
}