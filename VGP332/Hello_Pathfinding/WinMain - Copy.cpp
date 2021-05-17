#include "imgui/Inc/imgui.h"
#include "Klink/Inc/Klink.h"


using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;
using namespace Klink::Input;

using namespace Klink::AI;
using namespace Klink::AI::GraphSearch;


class GameState : public Klink::AppState
{
public:
	// he created this in main
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

};


int WINAPI WinMainOLD(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
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
	DebugUI::StaticInitialize(myWindow.GetWindowHandle(), true, true);
	Camera camera{};

	//////////// Start ////////////

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

	NavGraph graph;
	graph.Initialise(columns, rows, marginPadding, cellPadding, radius, bindNeighboursClockwise);
	

	// Two options
	BuildGraph(graph, columns, rows, marginPadding, cellPadding, radius);
	
	graph.DisconnectNode(55);
	graph.DisconnectNode(56);
	graph.DisconnectNode(57);
	graph.DisconnectNode(58);
	graph.DisconnectNode(59);
	graph.DisconnectNode(78);
	graph.DisconnectNode(101);

	graph.DisconnectNode(124);

	graph.DisconnectNode(147);
	graph.DisconnectNode(148);
	graph.DisconnectNode(149);
	graph.DisconnectNode(150);
	graph.DisconnectNode(151);
	graph.DisconnectNode(82);
	graph.DisconnectNode(105);
	graph.DisconnectNode(128);

	GraphSearchContext context;
	context.Init(columns * rows);

	std::list<size_t> path;


	////////////////
	// Timer
	Timer loopTimer{};
	loopTimer.Initialize();
	// End Timer
	//////////////////////////////////
	// 8 // 5
	//////////// GameLoop ////////////

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();
		GraphicsSystem::Get()->BeginRender(Colours::Black);
		loopTimer.Update();

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
			if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
			{
				graph.DisconnectAtPosition(inputSystem->GetMouseScreenX(), inputSystem->GetMouseScreenY());				
			}
			if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
			{
				// make a node valid (reconnect)
			}
			moveSpeed = 0.05f;
		}


		//////////// BFS ////////////
		size = rows * columns;
		ShowGraph(graph, radius);
		SimpleDraw::Render(camera);

		DebugUI::BeginRender();
		//////////// ImGui ////////////

		if (isFinding)
		{
			ImGui::GetStyle().Alpha = 0;
		}
		else
		{
			ImGui::GetStyle().Alpha = 1.0f;
		}

		ImGui::Text("FPS: %f", loopTimer.GetFPS());
		if(context.found)
			ImGui::Text("Node Found!");
		else
			ImGui::Text("Node not found!");
		if(ImGui::SliderInt("Rows", &rows, 2, 13))
			needsRebuild = true;
		if(ImGui::SliderInt("Columns", &columns, 2, 23))
			needsRebuild = true;
		if(ImGui::SliderInt("StartX", &startX, 0, 22))
			needsRebuild = true;
		if(ImGui::SliderInt("StartY", &startY, 0, 12))
			needsRebuild = true;
		if(ImGui::SliderInt("EndX", &endX, 0, 22))
			needsRebuild = true;
		if(ImGui::SliderInt("EndY", &endY, 0, 12))
			needsRebuild = true;
		if(ImGui::Checkbox("Build node neighbours clockwise?\n(Otherwise N-S-E-W-NE-SW-NW-SE)", &bindNeighboursClockwise))
			needsRebuild = true;


		if (ImGui::Button("Rebuild Graph") || needsRebuild)
		{
			needsRebuild = false;
			context.Reset();
			path.clear();
			context.Init(columns*rows);
			
			//BuildGraph(graph, columns, rows, marginPadding, cellPadding, radius, bindNeighboursClockwise);
		}
		if (ImGui::Button("Breadth First Search"))
		{
			isFinding = true;
			redrawReset = true;
			context.Reset();
			path.clear();
			context.startNode = startX + (startY*columns);
			context.endNode = endX + (endY*columns);

			BFS<NavGraph, CanOpenNode<NavGraph>> bfs(graph);
			bfs.Execute(context);

			if (context.found)
			{
				auto next = context.closedList.back();
				while (next != Klink::AI::InvalidNode)
				{
					path.push_front(next);
					next = context.parent[next];
				}
			}
		}
		if (ImGui::Button("Depth First Search"))
		{
			isFinding = true;
			redrawReset = true;
			context.Reset();
			path.clear();
			context.startNode = startX + (startY*columns);
			context.endNode = endX + (endY*columns);

			DFS<NavGraph, CanOpenNode<NavGraph>> dfs(graph);
			dfs.Execute(context);

			if (context.found)
			{
				auto next = context.closedList.back();
				while (next != Klink::AI::InvalidNode)
				{
					path.push_front(next);
					next = context.parent[next];
				}
			}
		}
		if (ImGui::Button("Dijkstra Search"))
		{
			isFinding = true;
			redrawReset = true;
			context.Reset();
			path.clear();
			context.startNode = startX + (startY*columns);
			context.endNode = endX + (endY*columns);

			Dijkstra<NavGraph, CanOpenNode<NavGraph>, PixelDistanceCostPolicy<NavGraph>> dijkstra(graph);
			dijkstra.Execute(context);

			if (context.found)
			{
				auto next = context.closedList.back();
				while (next != Klink::AI::InvalidNode)
				{
					path.push_front(next);
					next = context.parent[next];
				}
			}
		}
		if (ImGui::Button("A* Search"))
		{
			isFinding = true;
			redrawReset = true;
			context.Reset();
			path.clear();
			context.startNode = startX + (startY*columns);
			context.endNode = endX + (endY*columns);

			AStar<NavGraph, CanOpenNode<NavGraph>, PixelDistanceCostPolicy<NavGraph>> Astar(graph);
			Astar.Execute(context);

			if (context.found)
			{
				auto next = context.closedList.back();
				while (next != Klink::AI::InvalidNode)
				{
					path.push_front(next);
					next = context.parent[next];
				}
			}
		}

		ImGui::Text("Node Compare Count: %d", context.nodeCompares);
		DebugUI::EndRender();
		if (ShowPath(graph, context, path, 50, loopTimer, redrawReset))
		{
			isFinding = false;
		}
		redrawReset = false;

		GraphicsSystem::Get()->EndRender();
	}


	InputSystem::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}