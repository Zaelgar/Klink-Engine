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


	// Light
	mLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
	mLight.direction = Normalize({ 0.0f, -1.0f, 1.0f });


	// Material
	mMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
	mMaterial.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.power = { 10.0f };


	// Spacebox
	mSpaceBox = MeshBuilder::CreateUVSphere(500.0f, 32, 32);
	mSpaceBoxBuffer.Initialize(mSpaceBox);

	mSpaceBoxTexture = TextureManager::Get()->LoadTexture("Images/Earth/8k/stars_8k.jpg");

	mTextureVertexShader.Initialize("../../Assets/Shaders/Texturing.fx", VertexPX::Format);
	mTexturePixelShader.Initialize("../../Assets/Shaders/Texturing.fx");

	mConstantBuffer.Initialize(sizeof(Matrix4));


	// Earth
	mEarth = MeshBuilder::CreateSphere(4.0f, 128, 128);
	mEarthBuffer.Initialize(mEarth);

	mEarthDiffuseTexture = TextureManager::Get()->LoadTexture("Images/Earth/8k/earth_diffuse_8k.jpg");
	mEarthSpecularTexture = TextureManager::Get()->LoadTexture("Images/Earth/8k/earth_specular_8k.tif");
	mEarthBumpTexture = TextureManager::Get()->LoadTexture("Images/Earth/earth_bump.jpg");
	mEarthNormalTexture = TextureManager::Get()->LoadTexture("Images/Earth/8k/earth_normal_8k.tif");

	mStandardVertexShader.Initialize("../../Assets/Shaders/Standard_backup.fx", Vertex::Format);
	mStandardPixelShader.Initialize("../../Assets/Shaders/Standard_backup.fx");


	// Shaders and buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();

	//RasterizerStateManager::Get()->GetRasterizerState("Wireframe")->Set();
}

void GameState::Terminate()
{
	// Shader Buffers
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	// Earth
	mEarthBuffer.Terminate();
	mStandardPixelShader.Terminate();
	mStandardVertexShader.Terminate();

	// Spacebox
	mSpaceBoxBuffer.Terminate();
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

	const float moveSpeed = 15.0f;
	const float turnSpeed = 15.0f;
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

	// Sin flux thingy
	static float neg = 1.0f;
	sinModValue += deltaTime * neg;
	/*if (sinModValue >= 1.0f)
	{
		neg *= -1.0f;
	}*/

}

void GameState::Render()
{
	// Math first
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	auto world =
		Matrix4::RotationX(mRotation.x) *
		Matrix4::RotationY(mRotation.y) *
		Matrix4::RotationZ(mRotation.z) *
		Matrix4::Translation(mPosition);

	auto transform = Matrix4::Transpose(Matrix4::Translation(mCamera.GetPosition()) * view * proj);

	// Render Spacebox
	RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Set();

	mTextureVertexShader.Bind();
	mTexturePixelShader.Bind();

	TextureManager::Get()->GetTexture(mSpaceBoxTexture)->BindPS(0);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	mConstantBuffer.Set(&transform);
	mConstantBuffer.BindVS();

	mSpaceBoxBuffer.Render();
	RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Clear();

	// Render Earth
	mStandardVertexShader.Bind();
	mStandardPixelShader.Bind();

	TextureManager::Get()->GetTexture(mEarthDiffuseTexture)->BindPS(0);
	TextureManager::Get()->GetTexture(mEarthSpecularTexture)->BindPS(1);
	TextureManager::Get()->GetTexture(mEarthBumpTexture)->BindVS(2);
	TextureManager::Get()->GetTexture(mEarthNormalTexture)->BindPS(3);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	TransformData transformData;
	transformData.world = Matrix4::Transpose(world);
	transformData.wvp = Matrix4::Transpose(world * view * proj);
	transformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindPS(2);

	OptionsData optionsData;
	optionsData.displacementWeight = mDisplacementWeight;
	optionsData.sinMod = sinModValue;
	mOptionsBuffer.Set(optionsData);
	mOptionsBuffer.BindVS(3);

	mEarthBuffer.Render();

	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
	}
	if (ImGui::CollapsingHeader("Options"))
	{
		ImGui::DragFloat("Displacement Weight", &mDisplacementWeight);
	}
	if (ImGui::CollapsingHeader("Earth Position"))
	{
		ImGui::DragFloat3("Earth Translation", &mPosition.x, 0.3f);
		ImGui::DragFloat3("Earth Rotation", &mRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Directional Light Rotation"))
	{
		ImGui::DragFloat3("Rotation##LightRotation", &mLight.direction.x, 0.01f);
	}

	ImGui::End();
}