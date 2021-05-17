#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

std::filesystem::path meshPath = "../../Assets/Models/Imported/Zombie_Idle_30.klinkmodel";
std::filesystem::path matPath = "../../Assets/Models/Imported/Zombie_Idle_30.materials";

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 200.0f, -100.0f });
	mCamera.SetTarget({ 0.0f, 50.0f, 75.0f });

	MeshIO::ReadInMeshes(meshPath, mZombieModel);
	MeshIO::ReadInMaterials(matPath, mZombieModel);

	// Shaders and buffers
	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	// Transform Buffer
	mTransformBuffer.Initialize();

	// Light Buffer
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
	mDirectionalLight.direction = Normalize({ 0.0f, -1.0f, 1.0f });
	mLightBuffer.Initialize(mDirectionalLight);

	// Material Buffer
	mMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
	mMaterial.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.power = { 10.0f };
	mMaterialBuffer.Initialize(mMaterial);
	
	// Options Buffer
	mOptionsBuffer.Initialize();
}

void GameState::Terminate()
{
	// Zombie
	mZombieModel.Terminate();

	// Shader
	mVertexShader.Terminate();
	mPixelShader.Terminate();

	// Buffers
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mOptionsBuffer.Terminate();
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

	auto transform = Matrix4::Transpose(world * view * proj);

	// Set Buffers data
	TransformData transformData;
	transformData.world = Matrix4::Transpose(world);
	transformData.wvp = Matrix4::Transpose(world * view * proj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(transformData);

	// mLightBuffer already set in Initialise()
	// mMaterialBuffer already set in Initialise()

	OptionsData optionsData;
	optionsData.displacementWeight = 0.0f;
	mOptionsBuffer.Set(optionsData);

	// Bind Shaders and Buffers
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.BindVS(3);

	// Render Meshes (calling render on a model binds textures, samplers, and binds mesh buffers
	mZombieModel.Render();

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	

	ImGui::End();
}