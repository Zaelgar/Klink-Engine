#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;

void GameState::Initialize()
{
	// Physics Initialize w/ Settings
	PhysicsSettings pSet;
	pSet.gravity = Vector3(0.0f, -9.81f, 0.0f);
	pSet.IsScreenBoundary = true;
	mPhysicsWorld.Initialize(pSet);

	// Initialize Objects
	mObjects.resize(mObjectCount);

	int screenH = GraphicsSystem::Get()->GetBackBufferHeight();
	int screenW = GraphicsSystem::Get()->GetBackBufferWidth();
	int pointCount;
	Vector2 velocity;
	Vector2 position;
	float radius;

	for (auto& object : mObjects)
	{
		pointCount = (rand() % (maximumPoints - minimumPoints)) + minimumPoints;
		radius = (rand() % static_cast<int>(mMaximumRadius - mMinimumRadius)) + mMinimumRadius;
		position.x = (rand() % (screenW - static_cast<int>(2 * object.radius))) + object.radius;
		position.y = (rand() % (screenH - static_cast<int>(2 * object.radius))) + object.radius;
		velocity = RandomOnUnitCircle() * ((rand() % static_cast<int>(mMaximumSpeed)) + mMinimumSpeed);

		object.Initialize(pointCount, radius, position, velocity);

		mPhysicsWorld.AddObject(&object);
	}
}

void GameState::Terminate()
{
	mPhysicsWorld.Terminate();
	mObjects.clear();
}

void GameState::Update(float deltaTime)
{
	/// Input for 3D
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

	if (!isPaused)
	{
		mPhysicsWorld.Update(deltaTime);
	}
	for (auto& object : mObjects)
	{
		object.Update(deltaTime);
	}
}

void GameState::Render()
{
	for (auto& object : mObjects)
	{
		object.Draw();
		if (isDrawingAABB)
			object.DrawAABB();
		if (isDrawingVelocity)
			object.DrawVelocity();
		if (isDrawingCircleBoundingBox)
			object.DrawCircleBoundingBox();
	}

	SimpleDraw::DrawScreenDebugGrid(50, Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Pause Simulation", &isPaused);

	ImGui::Checkbox("Draw AABB", &isDrawingAABB);
	ImGui::Checkbox("Draw Circle Bounding Box", &isDrawingCircleBoundingBox);
	ImGui::Checkbox("Draw Velocity", &isDrawingVelocity);

	if (isSphereCollision)
	{
		if (ImGui::Button("Switch To Box Collision"))
		{
			isSphereCollision = false;
			isBoxCollision = true;
			mPhysicsWorld.ChangeCollisionType(Klink::Physics::CollisionType::AABB);

			isDrawingAABB = true;
			isDrawingCircleBoundingBox = false;
		}
	}
	if (isBoxCollision)
	{
		if (ImGui::Button("Switch To Sphere Collision"))
		{
			isBoxCollision = false;
			isSphereCollision = true;
			mPhysicsWorld.ChangeCollisionType(Klink::Physics::CollisionType::CircleBoundingBox);

			isDrawingCircleBoundingBox = true;
			isDrawingAABB = false;
		}
	}

	if (ImGui::Button("Change To Hull Generation State"))
	{
		KlinkApp::ChangeState("HullGenerationState");
	}
	ImGui::End();
}