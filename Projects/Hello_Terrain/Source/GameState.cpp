#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::ML;
using namespace Klink::Physics;

using namespace Klink::Terrain;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColour({ 0.25f, 0.25f, 0.25f });

	// Member initialize
	mCamera.SetPosition({ 0.0f, 15.0f, -30.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 1.0f });

	// Transform Buffer
	mTransformBuffer.Initialize();

	// Light Buffer
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
	mDirectionalLight.direction = Normalize({ 0.0f, -1.0f, 1.0f });
	mLightBuffer.Initialize(mDirectionalLight);

	mLightCamera.SetPosition(-mDirectionalLight.direction * 40.0f);
	mLightCamera.SetDirection(mDirectionalLight.direction);

	// Material Buffer
	mMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
	mMaterial.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	mMaterial.power = { 10.0f };
	mMaterialBuffer.Initialize(mMaterial);

	// Options Buffer
	mOptionsBuffer.Initialize();
	mSkyboxOptionsBuffer.Initialize();

	// Textures

	//Terrain/(DaylightBox)(RockCliff)(etc..)
	mTerrainLowDiffuse = TextureManager::Get()->LoadTexture("Terrain/Moss/moss_albedo.tif");
	mTerrainLowNormal = TextureManager::Get()->LoadTexture("Terrain/Moss/moss_normal.tif");

	mTerrainMidDiffuse = TextureManager::Get()->LoadTexture("Terrain/RockCliff/rockcliff_albedo.tif");
	mTerrainMidNormal = TextureManager::Get()->LoadTexture("Terrain/RockCliff/rockcliff_normal.tif");

	mTerrainHighDiffuse = TextureManager::Get()->LoadTexture("Terrain/Snow/snow_albedo.tif");
	mTerrainHighNormal = TextureManager::Get()->LoadTexture("Terrain/Snow/snow_normal.tif");


	// Skybox
	mSkyboxBuffer.Initialize();
	BaseMesh<VertexPX> cube = MeshBuilder::CreateSkycube(1000.0f);
	mSkyboxDiffuse = TextureManager::Get()->LoadTexture("Terrain/DaylightBox/DaylightBox_UV.png");
	mSkyboxMeshBuffer.Initialize(cube);

	// Shadow Mapping
	mDepthBuffer.Initialize();

	constexpr uint32_t shadowMapSize = 4096;
	mDepthTarget.Initialize(shadowMapSize, shadowMapSize, RenderTarget::Format::RGBA_F32);
}

void GameState::Terminate()
{
	// Buffers
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mOptionsBuffer.Terminate();

	mTerrainMeshBuffer.Terminate();
	mSkyboxBuffer.Terminate();

	mDepthBuffer.Terminate();
	mDepthTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
#pragma region Input
	{
		auto inputSystem = InputSystem::Get();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			KlinkApp::Shutdown();
			return;
		}

		float moveSpeed = 15.0f;
		float turnSpeed = 15.0f;
		if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		{
			moveSpeed = 60.0f;
		}
		else
		{
			moveSpeed = 15.0f;
		}

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

		if (inputSystem->IsKeyDown(KeyCode::X))
		{
			// Reset view
			Vector3 pos = { 0.0f, 5.0f, -10.0f };
			mCamera.SetPosition(pos);
			Vector3 look = { 0.0f, 0.0f, 1.0f };
			mCamera.SetTarget(look);
		}
	}
#pragma endregion

	if (mIsVisualised)
	{
		ErodeTerrain(mIterationsPerUpdate);
	}

	if (mIsDayCycling)
	{
		mDayTimeValue += deltaTime * mDaySpeed;
		float t = sin(mDayTimeValue);

		mDirectionalLight.diffuse = Lerp(mSunrise, mDay, t);
		mDirectionalLight.direction = Lerp(Vector3{0.f,-1.0f,1.0f}, Vector3{0.f, -1.0f, 0.0f}, t);
	}

	mLightCamera.SetDirection(mDirectionalLight.direction);
}

void GameState::Render()
{
	mDepthTarget.BeginRender(Colours::White);
	RenderDepthMap();
	mDepthTarget.EndRender();

	RenderScene();
}

void GameState::RenderDepthMap()
{
	// Math first
	Matrix4 view;
	Matrix4 proj;
	view = mLightCamera.GetViewMatrix();
	proj = mLightCamera.GetPerspectiveMatrix();

	/// Render Terrain
	auto terrainWorld = Matrix4::Translation(mTerrainPosition);

	ShaderManager::Get()->GetShader("DepthMappingShader")->Bind();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS();

	// Just throwing a new buffer up if we use depthmapping
	// TransformBuffer r(0)
	// matrix wvp
	Matrix4 wvp;
	wvp = Matrix4::Transpose(terrainWorld * view * proj);
	mDepthBuffer.Set(wvp);
	mDepthBuffer.BindVS(0);

	mTerrainMeshBuffer.Render();
}

void GameState::RenderScene()
{
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();
	auto world = Matrix4::Translation(mTerrainPosition);

	auto transform = Matrix4::Transpose(world * view * proj);

	// Set Buffers data
	TransformData transformData;
	transformData.world = Matrix4::Transpose(world);
	transformData.wvp = Matrix4::Transpose(world * view * proj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(transformData);

	OptionsData optionsData;

	optionsData.lowHeightLimit = 0.3f * mHeightScale;
	optionsData.lowScaling = 10.0f;
	optionsData.lowSlopeThreshold = 0.2f;

	optionsData.midHeightLimit = 0.7f * mHeightScale;
	optionsData.midScaling = 10.0f;
	optionsData.midSlopeThreshold = 0.2f;

	optionsData.highHeightLimit = mHeightScale;
	optionsData.highScaling = 10.0f;
	optionsData.highSlopeThreshold = 0.2f;

	optionsData.blendingAmount = mBlendingAmount;
	optionsData.sinFactor = mSinFactor;
	optionsData.snowHeightFactor = mSnowHeightFactor;

	mOptionsBuffer.Set(optionsData);

	if (mIsWireframe)
		RasterizerStateManager::Get()->GetRasterizerState("Wireframe")->Set();
	else
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	//ShaderManager::Get()->GetShader("TerrainShader")->Bind();
	//ShaderManager::Get()->GetShader("StandardShader")->Bind();
	ShaderManager::Get()->GetShader("TerrainTextureShader")->Bind();

	TextureManager::Get()->GetTexture(mTerrainLowDiffuse)->BindPS(0);
	TextureManager::Get()->GetTexture(mTerrainLowNormal)->BindPS(1);

	TextureManager::Get()->GetTexture(mTerrainMidDiffuse)->BindPS(2);
	TextureManager::Get()->GetTexture(mTerrainMidNormal)->BindPS(3);

	TextureManager::Get()->GetTexture(mTerrainHighDiffuse)->BindPS(4);
	TextureManager::Get()->GetTexture(mTerrainHighNormal)->BindPS(5);

	mDepthTarget.BindPS(6);

	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS(1);


	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mDirectionalLight.direction.Normalize();
	mLightBuffer.Set(mDirectionalLight);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.BindVS(3);
	mOptionsBuffer.BindPS(3);

	if (mCanRenderTerrain)
	{
		mTerrainMeshBuffer.Bind();
		mTerrainMeshBuffer.Render();
	}


	// Skybox - Moves with camera
	RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Set();
	ShaderManager::Get()->GetShader("SkyboxShader")->Bind();

	TextureManager::Get()->GetTexture(mSkyboxDiffuse)->BindPS();

	auto skyView = mCamera.GetViewMatrix();
	auto skyProj = mCamera.GetPerspectiveMatrix();
	auto skyWorld = Matrix4::Translation(mCamera.GetPosition());

	SkyboxData skyboxData;
	skyboxData.wvp = Matrix4::Transpose(skyWorld * skyView * skyProj);

	mSkyboxBuffer.Set(skyboxData);
	mSkyboxBuffer.BindVS();

	SkyboxOptions skyboxOptions;
	skyboxOptions.colourTint = mDirectionalLight.diffuse;

	mSkyboxOptionsBuffer.Set(skyboxOptions);
	mSkyboxOptionsBuffer.BindPS(1);

	mSkyboxMeshBuffer.Bind();
	mSkyboxMeshBuffer.Render();

	SimpleDraw::DrawSphere(mLightCamera.GetPosition(), 3.0f);

	SimpleDraw::DrawLine(Vector3::Zero(), mDirectionalLight.direction);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Terrain Settings"), true)
	{
		ImGui::DragFloat("Height Scale", &mHeightScale, 0.1f, 1.0f, 100.0f);
		ImGui::DragFloat("Size Scale", &mScale, 0.1f, 1.0f, 100.0f);
		ImGui::DragInt("Map Image To Load", &mMapType, 1, 1, 5);

		if (ImGui::Button("Build New Terrain"))
		{
			BuildTerrain(mMapType);
		}
	}

	if (ImGui::CollapsingHeader("Erosion Settings", true))
	{
		ImGui::DragInt("Iterations", &mNumErosionIterations, 100.0f, 1, 100000);
		ImGui::DragInt("Erosion Radius", &mErosionSettings.erosionRadius, 1, 2, 8);
		ImGui::DragFloat("Inertia", &mErosionSettings.inertia, 0.01f, 0.0f, 1.00f);
		ImGui::DragFloat("Sediment Capacity Factor", &mErosionSettings.sedimentCapacityFactor, 0.1f, 1.0f, 10.0f);
		ImGui::DragFloat("Minimum Sediment Capacity", &mErosionSettings.minSedimentCapacity, 0.001f, 0.001f, 2.0f);
		ImGui::DragFloat("Erode Speed", &mErosionSettings.erodeSpeed, 0.01f, 0.01f, 1.0f);
		ImGui::DragFloat("Deposit Speed", &mErosionSettings.depositSpeed, 0.01f, 0.01f, 1.0f);
		ImGui::DragFloat("Evaporate Speed", &mErosionSettings.evaporateSpeed, 0.01f, 0.01f, 1.0f);
		ImGui::DragFloat("Gravity Value", &mErosionSettings.gravity, 0.1f, 0.1f, 50.0f);
		ImGui::DragInt("Max Droplet Iterations", &mErosionSettings.maxDropletLifetime, 1, 5, 50);
		ImGui::DragFloat("Initial Droplet Volume", &mErosionSettings.initialWaterVolume, 0.1f, 0.1f, 5.0f);
		ImGui::DragFloat("Initial Speed", &mErosionSettings.initialSpeed, 0.1f, 0.1f, 30.0f);
		if (ImGui::Button("Erode Terrain") && mCanRenderTerrain)
		{
			ErodeTerrain(mNumErosionIterations);
		}
	}

	if (ImGui::CollapsingHeader("Visualization Settings"))
	{
		ImGui::DragInt("Iterations Per Update", &mIterationsPerUpdate, 1, 1, 10);
		if (ImGui::Button("Visualise over time?"))
		{
			mIsVisualised = true;
		}
	}

	if (ImGui::CollapsingHeader("Terrain Render Settings", true))
	{
		ImGui::Checkbox("Wireframe", &mIsWireframe);
		ImGui::DragFloat3("Terrain Position", &mTerrainPosition.x);
		ImGui::DragFloat3("Light Rotation", &mDirectionalLight.direction.x, 0.1f, -1.00f, 1.0f);
		ImGui::DragFloat("Blending Amount", &mBlendingAmount, 0.01f, 0.0f, 1.0f);
		ImGui::Checkbox("Day Cycle Visualise", &mIsDayCycling);
		ImGui::DragFloat("Skybox Tint Percent", &mTintPercent, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Day time value", &mDayTimeValue, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Sine factor", &mSinFactor, 0.1f, 0.01f, 5.0f);
		ImGui::DragFloat("Sine snow height factor", &mSnowHeightFactor, 0.1f, -10.0f, 10.0f);
		if (ImGui::Button("Reset Day Cycle"))
		{
			ResetDayCycle();
		}
	}

	//mAppLog.Draw("Console");
	ImGui::Image(mDepthTarget.GetShaderResourceView(), { 200.0f, 200.0f });
	ImGui::End();
}

void GameState::BuildTerrain(int type)
{
	mCanRenderTerrain = false;

	mTerrainMeshBuffer.Terminate();

	switch (type)
	{
	case 1:
		mFilename = "heightmap.png";
		break;
	case 2:
		mFilename = "ripple.png";
		break;
	case 3:
		mFilename = "bigimage2.png";
		break;
	case 4:
		mFilename = "image.png";
		break;
	case 5:
		mFilename = "bubble.png";
		break;
	}

	mHeightmap = mHeightmap.LoadHeightmapFromImage(mFilename.c_str());

	mHeightmap.NormalizeHeightValues();

	mTerrainMesh = MeshBuilder::CreateTerrain(mHeightmap.heightmap.data(), mHeightmap.xSize, mHeightmap.ySize, mScale, mHeightScale);
	mTerrainMeshBuffer.Initialize(mTerrainMesh, true);

	mCanRenderTerrain = true;
}

void GameState::ErodeTerrain(int numIterations)
{
	//auto time = std::clock();
	mErosion.SetErosionSettings(mErosionSettings);
	mErosion.Initialize(mHeightmap.xSize);
	mErosion.Erode(mTerrainMesh, mHeightmap.xSize, numIterations);

	MeshBuilder::CalculateNormals(mTerrainMesh);
	MeshBuilder::CalculateTangents(mTerrainMesh);

	mTerrainMeshBuffer.Update(mTerrainMesh.mVertices.size(), static_cast<void*>(mTerrainMesh.mVertices.data()));
}

void GameState::ResetDayCycle()
{
	mDayTimeValue = 0;

	mDirectionalLight.diffuse = mDay;
	mDirectionalLight.direction = Normalize({ 0.0f, -1.0f, 1.0f });
	mTintColour = mDay;
	mTintPercent = 0.1f;
}