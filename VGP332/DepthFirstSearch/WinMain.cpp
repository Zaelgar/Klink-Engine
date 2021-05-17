#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"


using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::Input;

using namespace Klink::AI;
using namespace Klink::AI::GraphSearch;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int screenW = 1280;
	int screenH = 720;

	float aspect = (float)screenW / (float)screenH;

	// Setup window, initialise the graphics system
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Pathfinding", screenW, screenH);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	SimpleDraw::StaticInitialize(50000);
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	DebugUI::StaticInitialize(myWindow.GetWindowHandle());

	Camera camera{};

	//////////// Start ////////////

	NavGraph graph;

	int columns = 23;
	int rows = 13;
	int size = 0;
	float radius = 20.0f;
	int cellPadding = 15;
	int marginPadding = 15;

	int startX = 1, startY = 4;
	int endX = 20, endY = 10;

	BuildGraph(graph, columns, rows, marginPadding, cellPadding, radius);

	GraphSearchContext context;
	context.Init(columns * rows);
	DFS<NavGraph, CanOpenNode<NavGraph>> dfs(graph);

	std::list<size_t> path;

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


		size = rows * columns;
		ShowGraph(graph, radius);


		if (path.size())
		{
			for (auto& node : path)
			{
				SimpleDraw::DrawScreenCircle(graph.GetNode(node).position, radius + 2.0f);
			}
		}


		SimpleDraw::Render(camera);
		DebugUI::BeginRender();
		//////////// ImGui ////////////
		ImGui::SliderInt("Rows", &rows, 2, 13);
		ImGui::SliderInt("Columns", &columns, 2, 23);

		ImGui::SliderInt("StartX", &startX, 0, 22);
		ImGui::SliderInt("StartY", &startY, 0, 12);

		ImGui::SliderInt("EndX", &endX, 0, 22);
		ImGui::SliderInt("EndY", &endY, 0, 12);

		if (ImGui::Button("Rebuild Graph"))
		{
			path.clear();
			BuildGraph(graph, columns, rows, marginPadding, cellPadding, radius);
			context.Init(columns*rows);
		}
		if (ImGui::Button("Breadth First Search"))
		{
			context.Reset();
			path.clear();
			context.startNode = startX + (startY*columns);
			context.endNode = endX + (endY*columns);

			GraphSearch::BFS<NavGraph, CanOpenNode<NavGraph>> bfs(graph);
			bfs.Execute(context);

			if (context.found)
			{
				auto next = context.closedList.back();
				while (next != InvalidNode)
				{
					path.push_front(next);
					next = context.parent[next];
				}
			}
		}
		DebugUI::EndRender();
		GraphicsSystem::Get()->EndRender();
	}

	InputSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}