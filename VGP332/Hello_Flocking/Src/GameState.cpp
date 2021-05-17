#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 1.0f });
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	SimpleDraw::DrawDebugGrid(100, {0.1f, 0.1f, 0.1f, 1.0f});
}