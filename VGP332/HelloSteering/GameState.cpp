#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	mAIWorld.AddObstacle(myOb);
	mAIWorld.AddObstacle(myOb2);
	mAIWorld.AddObstacle(myOb3);
	mAIWorld.AddObstacle(myOb4);
	mAIWorld.AddObstacle(myOb5);

	mAIWorld.AddWall(mTop);
	mAIWorld.AddWall(mBottom);
	mAIWorld.AddWall(mLeft);
	mAIWorld.AddWall(mRight);

	myFish = new Fish(mAIWorld);
	myFish->Initialize();
	myFish->position = Vector2(600.0f, 350.0f);

	myFish->destination.x = 600.0f;
	myFish->destination.y = 650.0f;

	//myShark = new Shark(mAIWorld);
	//myShark->Initialize();
	//myShark->position = Vector2(300.0f, 200.0f);

	mFishes.reserve(15);
	mFishes.resize(15);

	for (auto& fish : mFishes)
	{
		fish = new Fish(mAIWorld);
		fish->Initialize();
		fish->position = fish->RandomPositionOnScreen();
	}
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
	static bool updating = true;
	static bool debug = true;
	if (InputSystem::Get()->IsKeyPressed(KeyCode::U))
	{
		updating = updating ? false : true;
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::D))
	{
		debug = debug ? false : true;
	}	
	if (InputSystem::Get()->IsMousePressed(MouseButton::LBUTTON))
	{
		myFish->destination.x = InputSystem::Get()->GetMouseScreenX();
		myFish->destination.y = InputSystem::Get()->GetMouseScreenY();
	}

	for (auto& fish : mFishes)
	{
		fish->isDebugging = debug;
		fish->Update(deltaTime, updating);
	}
	//myShark->Update(deltaTime);
	myFish->isDebugging = debug;
	myFish->Update(deltaTime, updating);
}

void GameState::Render()
{
	myFish->Render(Colours::Blue);
	for (auto& fish : mFishes)
	{
		fish->Render(Colours::Yellow);
	}
	//myShark->Render(Colours::Red);

	SimpleDraw::DrawScreenCircle(myOb.center, myOb.radius, Colours::Green);
	SimpleDraw::DrawScreenCircle(myOb2.center, myOb2.radius, Colours::Green);
	SimpleDraw::DrawScreenCircle(myOb3.center, myOb3.radius, Colours::Green);
	SimpleDraw::DrawScreenCircle(myOb4.center, myOb4.radius, Colours::Green);
	SimpleDraw::DrawScreenCircle(myOb5.center, myOb5.radius, Colours::Green);

	SimpleDraw::DrawScreenLine(mTop.from, mTop.to, Colours::Green);
	SimpleDraw::DrawScreenLine(mBottom.from, mBottom.to, Colours::Green);
	SimpleDraw::DrawScreenLine(mLeft.from, mLeft.to, Colours::Green);
	SimpleDraw::DrawScreenLine(mRight.from, mRight.to, Colours::Green);

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
}