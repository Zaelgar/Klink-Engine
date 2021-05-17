#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;



void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 200.0f, -100.0f });
	mCamera.SetTarget({ 0.0f, 50.0f, 75.0f });

	mCube = MeshBuilder::CreatePCCube(5.0f, Colours::Aqua);
	mCubeBuffer.Initialize(mCube);

	mVertexColourShader.Initialize("../../Assets/Shaders/ColourShader.fx", VertexPC::Format);
	mPixelColourShader.Initialize("../../Assets/Shaders/ColourShader.fx");
}

void GameState::Terminate()
{
	mCubeBuffer.Terminate();
	mVertexColourShader.Terminate();
	mPixelColourShader.Terminate();
	mConstantBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	/// Input System
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


}

void GameState::Render()
{
	// Math first
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	auto world = Matrix4::Identity();

	auto transform = Matrix4::Transpose(Matrix4::Identity() * view * proj);

	mConstantBuffer.Set(transform);
	mConstantBuffer.BindVS();

	mVertexColourShader.Bind();
	mPixelColourShader.Bind();

	mCubeBuffer.Render();

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{}