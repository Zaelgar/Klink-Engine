#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int screenW = 1280;
	int screenH = 720;

	float aspect = (float)screenW / (float)screenH;

	// Setup window, initialise the graphics system
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello ImGUI", screenW, screenH);
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	DebugUI::StaticInitialize(myWindow.GetWindowHandle());

	// NOTE: constant buffer takes the matrices transposed!!

	//Camera camera(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), 0.1f, 1000.0f, 60.0f, screenH, screenW);
	Camera camera{};

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}

		const FLOAT clearColour[4]{ 0.2f,0.2f,0.2f,1.0f };
		GraphicsSystem::Get()->BeginRender(clearColour);

		DebugUI::BeginRender();
		ImGui::ShowDemoWindow();
		DebugUI::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}