#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

std::filesystem::path meshespath = "../../Assets/Models/Imported/Zombie_Idle_30.klinkmodel";
std::filesystem::path skeletonpath = "../../Assets/Models/Imported/Zombie_Idle_30.skeleton";
std::filesystem::path materialspath = "../../Assets/Models/Imported/Zombie_Idle_30.materials";
std::filesystem::path animationspath = "../../Assets/Models/Imported/Zombie_Idle_30.animationclips";

void DrawSkeleton(Skeleton& skeleton, std::vector<Matrix4> boneMatrices)
{
	for (int i = 0; i < boneMatrices.size(); ++i)
	{
		//Matrix4 m = boneMatrices[i];
		Matrix4 m = boneMatrices[i] * skeleton.bones[i].get()->offsetTransform;
		Vector3 v = Vector3::One();

		v *= m;

		SimpleDraw::DrawCircle(v, 0.1f);
	}
}

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 200.0f, -100.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 0.0f });

	// Shader stuff
	{
		// Light
		mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		mDirectionalLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
		mDirectionalLight.direction = Normalize({ 0.0f, -1.0f, 1.0f });

		// Material
		mMaterial.ambient = { 0.25f, 0.25f, 0.25f, 1.0f };
		mMaterial.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		mMaterial.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		mMaterial.power = { 10.0f };

		// Buffers
		mTransformBuffer.Initialize();
		mLightBuffer.Initialize(mDirectionalLight);
		mMaterialBuffer.Initialize(mMaterial);

		OptionsData displacment;
		displacment.displacementWeight = 0.0f;
		mOptionsBuffer.Initialize(displacment);

		mVertexShader.Initialize("../../Assets/Shaders/Skinning.fx", BoneVertex::Format);
		mPixelShader.Initialize("../../Assets/Shaders/Skinning.fx");
	}

	/// Model stuff
	MeshIO::ReadInMeshes(meshespath, mZombieModel);
	MeshIO::ReadInSkeleton(skeletonpath, mZombieModel);
	MeshIO::ReadInMaterials(materialspath, mZombieModel);
	MeshIO::ReadInAnimationClips(animationspath, mZombieIdleAnimation);

	boneMatrices = std::vector<Matrix4>(mZombieModel.skeleton.bones.size(), Matrix4::Identity());

	mBoneTransformBuffer.Initialize();

	mZombieAnimator.Initialize(&mZombieModel);
	mZombieAnimator.animationClip = &mZombieIdleAnimation;
}

void GameState::Terminate()
{
	mZombieModel.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mOptionsBuffer.Terminate();

	mBoneTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	/// Input System
	auto inputSystem = InputSystem::Get();
	{
		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			KlinkApp::Shutdown();
			return;
		}

		float moveSpeed = 15.0f;
		const float turnSpeed = 15.0f;
		if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
			moveSpeed = 120.0f;
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

	mZombieAnimator.Update(t, true);
}

void GameState::Render()
{
	// Math first
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();

	//Vector3 v = Vector3::One() * (1.0f / 10.0f);
	//auto world = Matrix4::Identity() * Matrix4::Scaling(v);
	auto world = Matrix4::Identity();

	mVertexShader.Bind();
	mPixelShader.Bind();

	TransformData transformData;
	transformData.world = Matrix4::Transpose(world);
	transformData.wvp = Matrix4::Transpose(world * view * proj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(transformData);

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.BindVS(3);

	BoneTransformData boneData;
	memcpy(&boneData.boneTransforms, boneMatrices.data(), boneMatrices.size() * sizeof(Matrix4));

	mBoneTransformBuffer.Set(boneData);
	mBoneTransformBuffer.BindVS(4);

	mZombieModel.Render();

	if (mShowSkeleton)
	{
		// auto& boneMatrices = mZombieAnimator.GetBoneMatrices();
		auto& boneMatrices = mZombieAnimator.GetBoneMatrices();
		// DrawSkeleton(mZombieModel.skeleton, boneMatrices);
		DrawSkeleton(mZombieModel.skeleton, boneMatrices);
	}

	// Debug Grid
	SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings");

	ImGui::DragFloat("T for animation", &t, 1.0f, 0.0f, 122.0f);
	
	if (ImGui::Button("Show Skeleton"))
	{
		mShowSkeleton = mShowSkeleton ? true : false;
	}

	ImGui::End();
}