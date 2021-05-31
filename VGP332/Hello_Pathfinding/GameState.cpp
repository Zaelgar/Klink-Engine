#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;

void GameState::Initialize()
{
	graph.Initialise(columns, rows, marginPadding, cellPadding, radius, bindNeighboursClockwise);

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

	context.Init(columns * rows);

	loopTimer.Initialize();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	#pragma region Input System
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
#pragma endregion

	mDeltaTime = deltaTime;
}

void GameState::Render()
{
	if (ShowPath(graph, context, path, 40, mDeltaTime, redrawReset))
	{
		isFinding = false;
	}
	redrawReset = false;

	ShowGraph(graph, 10.0f);

	SimpleDraw::DrawScreenCircle(graph.GetNode(static_cast<size_t>(startX + (startY * columns))).position, radius + 8.0f, Colours::Red);
	SimpleDraw::DrawScreenCircle(graph.GetNode(static_cast<size_t>(endX + (endY * columns))).position, radius + 8.0f, Colours::Green);

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Options");

	if (isFinding)
	{
		ImGui::GetStyle().Alpha = 0;
	}
	else
	{
		ImGui::GetStyle().Alpha = 1.0f;
	}

	ImGui::Text("FPS: %f", loopTimer.GetFPS());
	if (context.found)
		ImGui::Text("Node Found!");
	else
		ImGui::Text("Node not found!");
	if (ImGui::SliderInt("Rows", &rows, 2, 13))
		needsRebuild = true;
	if (ImGui::SliderInt("Columns", &columns, 2, 23))
		needsRebuild = true;
	if (ImGui::SliderInt("StartX", &startX, 0, 22))
		needsRebuild = true;
	if (ImGui::SliderInt("StartY", &startY, 0, 12))
		needsRebuild = true;
	if (ImGui::SliderInt("EndX", &endX, 0, 22))
		needsRebuild = true;
	if (ImGui::SliderInt("EndY", &endY, 0, 12))
		needsRebuild = true;
	if (ImGui::Checkbox("Build node neighbours clockwise?\n(Otherwise N-S-E-W-NE-SW-NW-SE)", &bindNeighboursClockwise))
		needsRebuild = true;


	if (ImGui::Button("Rebuild Graph") || needsRebuild)
	{
		needsRebuild = false;
		context.Reset();
		path.clear();
		context.Init(columns * rows);

		//BuildGraph(graph, columns, rows, marginPadding, cellPadding, radius, bindNeighboursClockwise);
	}
	if (ImGui::Button("Breadth First Search"))
	{
		isFinding = true;
		redrawReset = true;
		context.Reset();
		path.clear();
		context.startNode = startX + (startY * columns);
		context.endNode = endX + (endY * columns);

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
		context.startNode = startX + (startY * columns);
		context.endNode = endX + (endY * columns);

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
		context.startNode = startX + (startY * columns);
		context.endNode = endX + (endY * columns);

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
		context.startNode = startX + (startY * columns);
		context.endNode = endX + (endY * columns);

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

	ImGui::End();
}