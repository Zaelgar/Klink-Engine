#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

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
	myWindow.Initialize(hInstance, "Hello SpriteRenderer", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	SimpleDraw::StaticInitialize();
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	DebugUI::StaticInitialize(myWindow.GetWindowHandle());
	SpriteRenderer::StaticInitialize();

	Camera camera{};

	Texture earth{};
	earth.Initialize(L"../../Assets/SheepVsWolf/wolf_down_walk_01.png");

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();
		GraphicsSystem::Get()->BeginRender();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			done = true;
		}

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000;
		lastTime = currentTime;

	
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

		SimpleDraw::DrawDebugGrid();
		SimpleDraw::Render(camera);

		DebugUI::BeginRender();
		Matrix4 test = camera.GetViewMatrix();
		ImGui::Text
		(
			"_11: %9f \t_12: %f \t_13 %f \t_14 %f\n_21: %f \t_22: %f \t_23 %f \t_24 %f\n_31: %f \t_32: %f \t_33 %f \t_34 %f\n_41: %f \t_42: %f \t_43 %f \t_44 %f",
			test._11, test._12, test._13, test._14,
			test._21, test._22, test._23, test._24,
			test._31, test._32, test._33, test._34,
			test._41, test._42, test._43, test._44
		);
		DebugUI::EndRender();

		SpriteRenderer::Get()->BeginRender();
		SpriteRenderer::Get()->Draw(earth, {});
		SpriteRenderer::Get()->EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	earth.Terminate();

	InputSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}