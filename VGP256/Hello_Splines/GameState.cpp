#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;

void DrawCircle(Circle& circle, Vector4 colour = Colours::Green)
{
	SimpleDraw::DrawScreenCircle(circle.center, circle.radius, colour);
}

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 30.0f, -20.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 0.0f });

	mPositions.resize(6);

	mPositions[0] = catmulPos1;
	mPositions[1] = catmulPos2;
	mPositions[2] = catmulPos3;
	mPositions[3] = catmulPos4;
	mPositions[4] = catmulPos5;
	mPositions[5] = catmulPos6;
}

void GameState::Terminate()
{
	
}

void GameState::Update(float deltaTime)
{
	/// Input
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

	// These for loops calculate the spline at two points (at t, and at t+1) and draws a line between them
	// This happens 100 times to give the impression of a smooth line
	Vector2 p1;
	Vector2 p2;

	/// Quadratic Bezier
	if (isDrawingQuadratic)
	{
		// Draw points of curve
		SimpleDraw::DrawScreenCircle(qPos1, 10.0f, Colours::Green);
		SimpleDraw::DrawScreenCircle(qPos2, 10.0f, Colours::Green);
		SimpleDraw::DrawScreenCircle(qPos3, 10.0f, Colours::Green);

		for (int i = 0; i < 100; ++i)							// Quadratic Bezier
		{
			float t = (static_cast<float>(i) / 100.0f);
			float t2 = (static_cast<float>(i + 1) / 100.0f);

			p1 = QuadraticBezierCurveLerp(qPos1, qPos2, qPos3, t);
			p2 = QuadraticBezierCurveLerp(qPos1, qPos2, qPos3, t2);

			SimpleDraw::DrawScreenLine(p1, p2, Colours::Green);
		}
	}

	/// Cubic Bezier
	if (isDrawingCubic)
	{
		// Draw points of curve
		SimpleDraw::DrawScreenCircle(cPos1, 10.0f, Colours::Yellow);
		SimpleDraw::DrawScreenCircle(cPos2, 10.0f, Colours::Yellow);
		SimpleDraw::DrawScreenCircle(cPos3, 10.0f, Colours::Yellow);
		SimpleDraw::DrawScreenCircle(cPos4, 10.0f, Colours::Yellow);

		for (int i = 0; i < 100; ++i)							// Cubic Bezier
		{
			float t = (static_cast<float>(i) / 100.0f);
			float t2 = (static_cast<float>(i + 1) / 100.0f);

			p1 = CubicBezierCurveLerp(cPos1, cPos2, cPos3, cPos4, t);
			p2 = CubicBezierCurveLerp(cPos1, cPos2, cPos3, cPos4, t2);

			SimpleDraw::DrawScreenLine(p1, p2, Colours::Yellow);
		}
	}

	/// Catmull-Rom
	if (isDrawingCatmullRom)
	{
		// Draw points of the spline
		SimpleDraw::DrawScreenCircle(catmulPos1, 10.0f, Colours::Red);
		SimpleDraw::DrawScreenCircle(catmulPos2, 10.0f, Colours::Red);
		SimpleDraw::DrawScreenCircle(catmulPos3, 10.0f, Colours::Red);
		SimpleDraw::DrawScreenCircle(catmulPos4, 10.0f, Colours::Red);
		SimpleDraw::DrawScreenCircle(catmulPos5, 10.0f, Colours::Red);
		SimpleDraw::DrawScreenCircle(catmulPos6, 10.0f, Colours::Red);

		// Make sure our vector is updated (in case the points are changed in the Debug UI)
		mPositions[0] = catmulPos1;
		mPositions[1] = catmulPos2;
		mPositions[2] = catmulPos3;
		mPositions[3] = catmulPos4;
		mPositions[4] = catmulPos5;
		mPositions[5] = catmulPos6;

		for (int i = 0; i < 100; ++i)
		{
			// t is 0-5 for 6 points
			float t = static_cast<float>(i) * (5.0f / 100.0f);
			float t2 = static_cast<float>(i+1) * (5.0f / 100.0f);

			p1 = CatmullRomSplineLerp(mPositions, t);
			p2 = CatmullRomSplineLerp(mPositions, t2);

			SimpleDraw::DrawScreenLine(p1, p2, Colours::Red);
		}
	}
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Options");
	ImGui::Checkbox("Draw Quadratic Bezier", &isDrawingQuadratic);
	ImGui::Checkbox("Draw Cubic Bezier", &isDrawingCubic);
	ImGui::Checkbox("Draw Catmull-Rom Spline", &isDrawingCatmullRom);
	ImGui::End();
	
	if (isDrawingQuadratic)
	{
		ImGui::Begin("Quadratic Bezier Settings");
		ImGui::DragFloat2("First Quadratic Position", &qPos1.x);
		ImGui::DragFloat2("Second Quadratic Position", &qPos2.x);
		ImGui::DragFloat2("Third Quadratic Position", &qPos3.x);
		ImGui::End();
	}

	if (isDrawingCubic)
	{
		ImGui::Begin("Cubic Bezier Settings");
		ImGui::DragFloat2("First Cubic Position", &cPos1.x);
		ImGui::DragFloat2("Second Cubic Position", &cPos2.x);
		ImGui::DragFloat2("Third Cubic Position", &cPos3.x);
		ImGui::DragFloat2("Fourth Cubic Position", &cPos4.x);
		ImGui::End();
	}

	if (isDrawingCatmullRom)
	{
		ImGui::Begin("Catmull-Rom Splines");
		ImGui::DragFloat2("First Catmull-Rom Position", &catmulPos1.x);
		ImGui::DragFloat2("Second Catmull-Rom Position", &catmulPos2.x);
		ImGui::DragFloat2("Third Catmull-Rom Position", &catmulPos3.x);
		ImGui::DragFloat2("Fourth Catmull-Rom Position", &catmulPos4.x);
		ImGui::DragFloat2("Fifth Catmull-Rom Position", &catmulPos5.x);
		ImGui::DragFloat2("Sixth Catmull-Rom Position", &catmulPos6.x);
		ImGui::End();
	}
}