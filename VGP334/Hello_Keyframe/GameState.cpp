#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

std::filesystem::path path = "../../Assets/Models/Imported/zombie.klinkmodel";

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 5.0f, -5.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 0.0f });

	// Cube
	mCubeTexture = TextureManager::Get()->LoadTexture("Images/Brick/brick_diffuse.tif");

	mTextureVertexShader.Initialize("../../Assets/Shaders/Texturing.fx", VertexPX::Format);
	mTexturePixelShader.Initialize("../../Assets/Shaders/Texturing.fx");

	mCube = MeshBuilder::CreateUVCube(1.0f);
	mCubeBuffer.Initialize(mCube);

	// Shaders and buffers
	mTransformBuffer.Initialize();



	// Build Animation - Simple looping animation
	animBuilder
		.AddPositionKey(Vector3::Zero(), 0.0f).AddRotationKey(Quaternion::Identity(), 0.0f).AddScalingKey(Vector3::One(), 0.0f)
		.AddPositionKey(Vector3(10.0f, 0.0f, 0.0f), 5.0f).AddRotationKey(EulerDegreesToQuaternion(0.0f, 90.0f, 0.0f), 5.0f).AddScalingKey(Vector3(2.0f, 1.0f, 1.0f), 5.0f)
		.AddKey(Vector3::Zero(), Quaternion::Identity(), Vector3::One(), 10.0f);
	
	cubeAnimation = animBuilder.Get();

	Animator animator;
}

void GameState::Terminate()
{
	// Shader
	mTransformBuffer.Terminate();

	// Cube
	mCubeBuffer.Terminate();

	mTextureVertexShader.Terminate();
	mTexturePixelShader.Terminate();
}

void GameState::Update(float deltaTime)
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
	if(inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if(inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	// Math first
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	mRotation = EulerDegreesToQuaternion(mRotationEuler.x, mRotationEuler.y, mRotationEuler.z);

	auto world = cubeAnimation.GetTransform(mTime);

	TransformData transform;
	transform.wvp = Matrix4::Transpose(world * view * proj);

	// Render cube Mesh
	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	mTransformBuffer.Set(transform);
	mTransformBuffer.BindVS();

	TextureManager::Get()->GetTexture(mCubeTexture)->BindPS(0);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	mCubeBuffer.Render();

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	ImGui::DragFloat("Time", &mTime, 0.1f, 0.0f, 10.0f);

	ImGui::End();
}