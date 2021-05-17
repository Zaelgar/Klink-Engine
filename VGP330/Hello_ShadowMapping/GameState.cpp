#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	// Camera
	//mMainCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	//mMainCamera.SetTarget({ 0.0f, 0.0f, 1.0f });


	// Light
	mLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
	mLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	// Light Camera for ShadowMapping
	mLightCamera.SetPosition(-mLight.direction * 30.0f);
	mLightCamera.SetTarget(mLight.direction);

	// Material
	mMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
	mMaterial.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.power = { 10.0f };

	// Cube
	mCubePosition.x = 0.0f;
	mCubePosition.y = 3.0f;
	mCubePosition.z = 0.0f;
	mCube = MeshBuilder::CreateCube(mCubeSize);
	mCubeBuffer.Initialize(mCube);

	mCubeDiffuseTexture = TextureManager::Get()->LoadTexture("Images/Brick/brick_diffuse.tif");
	mCubeSpecularTexture = TextureManager::Get()->LoadTexture("Images/Brick/brick_specular.tif");
	mCubeBumpTexture = TextureManager::Get()->LoadTexture("Images/Brick/brick_heightmap.tif");
	mCubeNormalTexture = TextureManager::Get()->LoadTexture("Images/Brick/brick_normal.tif");

	// Plane
	mPlanePosition.x = 0.0f;
	mPlanePosition.y = 0.1f;
	mPlanePosition.z = 0.0f;
	mPlane = MeshBuilder::CreateQuad(mPlaneX, mPlaneY);
	mPlaneBuffer.Initialize(mPlane);

	mPlaneDiffuseTexture = TextureManager::Get()->LoadTexture("Images/Rock/rock_diffuse.tif");
	mPlaneNormalTexture = TextureManager::Get()->LoadTexture("Images/Rock/rock_normal.tif");
	mPlaneSpecularTexture = TextureManager::Get()->LoadTexture("Images/Rock/rock_specular.tif");
	mPlaneBumpTexture = TextureManager::Get()->LoadTexture("Images/Rock/rock_heightmap.tif");

	// Shaders and buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();


	// POST PROCESSING
	auto graphicsSystem = GraphicsSystem::Get();

	graphicsSystem->SetClearColour(Colours::Black);


	mDepthBuffer.Initialize();

	constexpr uint32_t shadowMapSize = 4096;
	mDepthTarget.Initialize(
		shadowMapSize,
		shadowMapSize,
		RenderTarget::Format::RGBA_F32); // This matters.... truncation and precision
}

void GameState::Terminate()
{
	// Shader Buffers
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	// Cube
	mCubeBuffer.Terminate();

	// Plane
	mPlaneBuffer.Terminate();

	// Shadow Mapping RT
	mDepthBuffer.Terminate();
	mDepthTarget.Terminate();
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

		const float moveSpeed = 15.0f;
		const float turnSpeed = 15.0f;
		if (inputSystem->IsKeyDown(KeyCode::W))
			mMainCamera.Walk(moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::A))
			mMainCamera.Strafe(-moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::S))
			mMainCamera.Walk(-moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::D))
			mMainCamera.Strafe(moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::Q))
			mMainCamera.Rise(moveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::E))
			mMainCamera.Rise(-moveSpeed * deltaTime);
		if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		{
			mMainCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
			mMainCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
		}
	}
}

void GameState::Render()
{
	mDepthTarget.BeginRender(Colours::White);
	RenderDepthMap();
	mDepthTarget.EndRender();

	RenderScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	ImGui::Checkbox("Use Light Camera?", &mIsUsingLightCamera);
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
	if (ImGui::CollapsingHeader("Cube Position"))
	{
		ImGui::DragFloat3("Cube Translation", &mCubePosition.x, 0.3f);
		ImGui::DragFloat3("Cube Rotation", &mCubeRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Plane Position"))
	{
		ImGui::DragFloat3("Plane Translation", &mPlanePosition.x, 0.3f);
		ImGui::DragFloat3("Plane Rotation", &mPlaneRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Directional Light Rotation"))
	{
		ImGui::DragFloat3("Rotation##LightRotation", &mLight.direction.x, 0.01f);
	}

	ImGui::Image(mDepthTarget.GetShaderResourceView(), {200.0f, 200.0f});

	ImGui::End();
}

void GameState::RenderScene()
{
	// Math first
	auto view = mMainCamera.GetViewMatrix();
	auto proj = mMainCamera.GetPerspectiveMatrix();

	auto lightView = mLightCamera.GetViewMatrix();
	auto lightProj = mLightCamera.GetPerspectiveMatrix();

	/// Render Cube
	auto cubeWorld =
		Matrix4::RotationX(mCubeRotation.x) *
		Matrix4::RotationY(mCubeRotation.y) *
		Matrix4::RotationZ(mCubeRotation.z) *
		Matrix4::Translation(mCubePosition);

	ShaderManager::Get()->GetShader("StandardShader")->Bind();

	TextureManager::Get()->GetTexture(mCubeDiffuseTexture)->BindPS(0);
	TextureManager::Get()->GetTexture(mCubeNormalTexture)->BindPS(1);
	TextureManager::Get()->GetTexture(mCubeSpecularTexture)->BindPS(2);
	TextureManager::Get()->GetTexture(mCubeBumpTexture)->BindVS(3);
	mDepthTarget.BindPS(4);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS(1);	// DepthMap Sampler

	TransformData transformData;
	transformData.world = Matrix4::Transpose(cubeWorld);
	transformData.wvp = Matrix4::Transpose(cubeWorld * view * proj);
	transformData.lightWvp = Matrix4::Transpose(cubeWorld * lightView * lightProj);

	if (!mIsUsingLightCamera)
		transformData.viewPosition = mMainCamera.GetPosition();
	else
		transformData.viewPosition = mLightCamera.GetPosition();

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
	mOptionsBuffer.Set(optionsData);
	mOptionsBuffer.BindVS(3);

	mCubeBuffer.Render();
	/// End Render cube

	/// Plane
	auto planeWorld =
		Matrix4::RotationX(mPlaneRotation.x) *
		Matrix4::RotationY(mPlaneRotation.y) *
		Matrix4::RotationZ(mPlaneRotation.z) *
		Matrix4::Translation(mPlanePosition);

	transformData.world = Matrix4::Transpose(planeWorld);
	transformData.wvp = Matrix4::Transpose(planeWorld * view * proj);
	transformData.lightWvp = Matrix4::Transpose(planeWorld * lightView * lightProj);

	if (!mIsUsingLightCamera)
		transformData.viewPosition = mMainCamera.GetPosition();
	else
		transformData.viewPosition = mLightCamera.GetPosition();

	ShaderManager::Get()->GetShader("StandardShader")->Bind();

	TextureManager::Get()->GetTexture(mPlaneDiffuseTexture)->BindPS(0);
	TextureManager::Get()->GetTexture(mPlaneNormalTexture)->BindPS(1);
	TextureManager::Get()->GetTexture(mPlaneSpecularTexture)->BindPS(2);
	TextureManager::Get()->GetTexture(mPlaneBumpTexture)->BindVS(3);
	mDepthTarget.BindPS(4);
	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS(1);

	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS();
	mTransformBuffer.BindPS(0);

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.Set(optionsData);
	mOptionsBuffer.BindVS(3);

	mPlaneBuffer.Render();

	/// Debug Grid	
	SimpleDraw::DrawSphere(mLightCamera.GetPosition(), 1.0f);
	SimpleDraw::DrawLine(mLightCamera.GetPosition(), mLightCamera.GetDirection() * 10.0f);

	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mMainCamera);
}

void GameState::RenderDepthMap()
{
	// Math first
	Matrix4 view;
	Matrix4 proj;
	view = mLightCamera.GetViewMatrix();
	proj = mLightCamera.GetPerspectiveMatrix();

	/// Render Cube
	auto cubeWorld =
		Matrix4::RotationX(mCubeRotation.x) *
		Matrix4::RotationY(mCubeRotation.y) *
		Matrix4::RotationZ(mCubeRotation.z) *
		Matrix4::Translation(mCubePosition);

	ShaderManager::Get()->GetShader("DepthMappingShader")->Bind();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS();

	// Just throwing a new buffer up if we use depthmapping
	// TransformBuffer r(0)
	// matrix wvp
	Matrix4 wvp;
	wvp = Matrix4::Transpose(cubeWorld * view * proj);
	mDepthBuffer.Set(wvp);
	mDepthBuffer.BindVS(0);

	mCubeBuffer.Render();

	/// Plane
	auto planeWorld =
		Matrix4::RotationX(mPlaneRotation.x) *
		Matrix4::RotationY(mPlaneRotation.y) *
		Matrix4::RotationZ(mPlaneRotation.z) *
		Matrix4::Translation(mPlanePosition);

	ShaderManager::Get()->GetShader("DepthMappingShader")->Bind();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS();

	wvp = Matrix4::Transpose(planeWorld * view * proj);
	mDepthBuffer.Set(wvp);
	mDepthBuffer.BindVS(0);

	mPlaneBuffer.Render();
}