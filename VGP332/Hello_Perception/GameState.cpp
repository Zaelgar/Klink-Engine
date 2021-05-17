#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	mySheep = new Sheep(myWorld);
	mySheep->Initialize();
}

void GameState::Terminate()
{
	delete mySheep;
	mySheep = nullptr;
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		KlinkApp::Shutdown();
		return;
	}

	mySheep->Update(deltaTime);
}

void GameState::Render()
{
	auto sr = SpriteRenderer::Get();
	sr->BeginRender();

	mySheep->Render();

	sr->EndRender();
}

void GameState::DebugUI()
{
	
}