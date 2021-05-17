#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 5.0f, -5.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 0.0f });

	PhysicsSettings pSet;

	mPhysicsWorld.Initialize(pSet);

	srand(static_cast<unsigned int>(time(NULL)));
}

void GameState::Terminate()
{
	
}

void GameState::Update(float deltaTime)
{
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

	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Boom!"))
	{
		mPhysicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			auto vel = RandomOnUnitSphere() * 0.1f;
			vel.y = (Abs(vel.y));

			auto particle = new Particle3();
			particle->SetPosition(Vector3::Zero());
			particle->SetVelocity(vel);
			particle->radius = 0.05f;
			particle->invMass = 1.0f;
			mPhysicsWorld.AddParticle(particle);
		}
	}
	ImGui::End();
}