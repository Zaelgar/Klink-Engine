#include "imgui/Inc/imgui.h"
#include "../Engine/Klink/Inc/Klink.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::Input;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int screenW = 1280;
	int screenH = 720;

	float aspect = (float)screenW / (float)screenH;

	// Setup window, initialise the graphics system
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Screen Draw", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	SimpleDraw::StaticInitialize();
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	DebugUI::StaticInitialize(myWindow.GetWindowHandle());

	Camera camera{};

	int topLeftX = 1;
	int topLeftY = 1;
	int botRightX = 10;
	int botRightY = 10;

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();
		GraphicsSystem::Get()->BeginRender(Colours::Black);

		// Input
		{
			if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
			{
				done = true;
			}

			float moveSpeed = 0.05f;
			const float turnSpeed = 0.001f;
			if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
			{
				moveSpeed *= 3;
			}
			if (inputSystem->IsKeyDown(KeyCode::Q))
			{
				camera.Rise(moveSpeed);
			}
			if (inputSystem->IsKeyDown(KeyCode::E))
			{
				camera.Rise(-moveSpeed);
			}
			if (inputSystem->IsKeyDown(KeyCode::W))
			{
				camera.Walk(moveSpeed);
			}
			if (inputSystem->IsKeyDown(KeyCode::S))
			{
				camera.Walk(-moveSpeed);
			}
			if (inputSystem->IsKeyDown(KeyCode::A))
			{
				camera.Strafe(moveSpeed);
			}
			if (inputSystem->IsKeyDown(KeyCode::D))
			{
				camera.Strafe(-moveSpeed);
			}
			if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
			{
				camera.Yaw(inputSystem->GetMouseMoveX() * -turnSpeed);
				camera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed);
			}
			moveSpeed = 0.05f;
		}


		DebugUI::BeginRender();
		ImGui::SliderInt("Rectangle TopLeft X", &topLeftX, 1, 1279);
		ImGui::SliderInt("Rectangle TopLeft Y", &topLeftY, 1, 719);
		ImGui::SliderInt("Rectangle BotLeft X", &botRightX, 1, 1279);
		ImGui::SliderInt("Rectangle BotLeft Y", &botRightY, 1, 719);
		DebugUI::EndRender();

		SimpleDraw::DrawScreenCircle({640, 360}, 100.0f, Colours::Green);
		SimpleDraw::DrawScreenSquare({640, 360}, 300.0f, Colours::Red);
		SimpleDraw::DrawScreenRect({ (float)topLeftX, (float)topLeftY }, { (float)botRightX, (float)botRightY}, Colours::Blue);
		SimpleDraw::DrawScreenLine({640, 360}, {640, 700}, Colours::White);

		SimpleDraw::Render(camera);

		GraphicsSystem::Get()->EndRender();
	}

	InputSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}