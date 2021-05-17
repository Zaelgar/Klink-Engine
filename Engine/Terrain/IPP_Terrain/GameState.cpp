#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 1.0f });

	mSphere = MeshBuilder::CreateUVSphere(5.0f, 3, 2);
	mSphereBuffer.Initialize(mSphere);

	mVertexShader.Initialize("../../Assets/Shaders/Texturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Texturing.fx");
	mConstantBuffer.Initialize(sizeof(Matrix4));
	mTexture.Initialize(L"../../Assets/earth.jpg");
}

void GameState::Terminate()
{
	mTexture.Terminate();
	mConstantBuffer.Terminate();
	mSphereBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		KlinkApp::Shutdown();
		return;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 0.5f;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * -turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	mTexture.BindPS();
	mVertexShader.Bind();
	mPixelShader.Bind();

	//auto world = Matrix4::Translation(mPosition);
	auto world = Matrix4::Identity();
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	Matrix4 trans = Matrix4::Transpose(mTransform * view * proj);

	mConstantBuffer.Set(&trans);
	mConstantBuffer.BindVS();

	mSphereBuffer.Render();

	SimpleDraw::DrawDebugGrid();
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
}