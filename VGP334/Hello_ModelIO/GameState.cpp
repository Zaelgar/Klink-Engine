#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

std::filesystem::path path = "../../Assets/Models/Imported/Zombie_Idle_30.klinkmodel";

void GameState::Initialize()
{
	MeshIO::ReadInMeshes(path, zombieModel);

	// Camera
	mCamera.SetPosition({ 0.0f, 200.0f, -100.0f });
	mCamera.SetTarget({ 0.0f, 50.0f, 75.0f });

	// Zombie
	mZombieDiffuseOne = TextureManager::Get()->LoadTexture("Models/Imported/Zombie_Idle_30_mat_0_diffuse_0.png");
	mZombieDiffuseTwo = TextureManager::Get()->LoadTexture("Models/Imported/Zombie_Idle_30_mat_1_diffuse_0.png");

	mTextureVertexShader.Initialize("../../Assets/Shaders/TexturingOptions.fx", BoneVertex::Format);
	mTexturePixelShader.Initialize("../../Assets/Shaders/TexturingOptions.fx");

	// Shaders and buffers
	mTransformBuffer.Initialize();
	mOptionsBuffer.Initialize();
}

void GameState::Terminate()
{
	// Shader
	mTransformBuffer.Terminate();
	mOptionsBuffer.Terminate();

	// Zombie
	zombieModel.meshes[0].meshBuffer.Terminate(); // HACK
	zombieModel.meshes[1].meshBuffer.Terminate();

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

	auto world = Matrix4::Identity() *(1.0f / 10.0f);

	auto transform = Matrix4::Transpose(world * view * proj);

	// Render Zombie mesh 1
	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	TextureManager::Get()->GetTexture(mZombieDiffuseOne)->BindPS(0);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	
	TransformData transformData;
	transformData.wvp = transform;

	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS();

	OptionsData options;
	options.uTranslate = uTranslate;
	options.vTranslate = vTranslate;

	mOptionsBuffer.Set(options);
	mOptionsBuffer.BindPS(1);

	zombieModel.meshes[0].meshBuffer.Render();

	// Render Zombie mesh 2
	TextureManager::Get()->GetTexture(mZombieDiffuseTwo)->BindPS(0);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	zombieModel.meshes[1].meshBuffer.Render();

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	if (ImGui::CollapsingHeader("UV Translate"))
	{
		ImGui::DragFloat("U Translate", &uTranslate, 0.01f);
		ImGui::DragFloat("V Translate", &vTranslate, 0.01f);
	}

	ImGui::End();
}