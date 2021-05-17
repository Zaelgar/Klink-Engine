#include "GameState.h"

#include <imgui/Inc/implot.h>

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::Physics;
using namespace Klink::ML;

void GameState::Initialize()
{
	// Camera
	mCamera.SetPosition({ 0.0f, 5.0f, -5.0f });
	mCamera.SetTarget({ 0.0f, 0.0f, 0.0f });
	srand(static_cast<unsigned int>(time(NULL)));

	mDatasetProperties.numSamples = 1000;
	mDatasetProperties.b[0] = 0.0f;
	mDatasetProperties.b[1] = 0.0f;
	mDatasetProperties.minX = 0.0f;
	mDatasetProperties.maxX = 100.0f;
	mDatasetProperties.noise = 10.0f;

	mDataset = Datasets::MakeLinear(mDatasetProperties);
	//mDataset = Datasets::MakeLinear(sampleCount, b0Value, b1Value, minX, maxX, noise);
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
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

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
	}

	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
	{
		mAppLog.AddLog("Bam!\n");
	}

	while (canRunAgain)
	{
		float error = 0.0f;
		for (int e = 0; e < epoch; ++e)
		{
			error = mLinReg.LearnToFit(mDataset);
			mAppLog.AddLog("b0 = %f, b1 = %f, error = %f\n", mLinReg.b0, mLinReg.b1, error);
		}

		canRunAgain = false;
	}
}

void GameState::Render()
{
	// Debug Grid
	//SimpleDraw::DrawDebugGrid(100, Colours::DarkSlateGray);
	//SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	if (ImGui::Button("Run"))
	{
		mLinReg.b0 = 0.0f;
		mLinReg.b1 = 0.0f;
		canRunAgain = true;
		mDatasetProperties.numSamples = sampleCount;	// ImGui cannot take size_t as a drag int
		mDataset = Datasets::MakeLinear(mDatasetProperties);
	}

	ImGui::SliderFloat("Learning Rate", &mLinReg.learningRate, 0.000001f, 0.001f,"%.7f");
	ImGui::SliderInt("Epochs", &epoch, 100, 100000);
	ImGui::SliderInt("Sample Count", &sampleCount, 10, 1000);
	ImGui::DragFloat("B0 Value", &mDatasetProperties.b[0], 0.1f);
	ImGui::DragFloat("B1 Value", &mDatasetProperties.b[1], 0.1f);
	ImGui::DragFloat("minX", &mDatasetProperties.minX, 0.1f);
	ImGui::DragFloat("maxX", &mDatasetProperties.maxX, 0.1f);
	ImGui::DragFloat("noise", &mDatasetProperties.noise, 0.1f);

	if (ImPlot::BeginPlot("Linear Regression", "X", "y"))
	{
		float startX = 0.0f;
		float endX = 100.0f;
		float startY = mLinReg.Predict(startX);
		float endY = mLinReg.Predict(endX);

		std::vector<ImVec2> line;
		line.push_back({ startX, startY });
		line.push_back({ endX, endY });

		ImPlot::PlotScatter("Dataset", mDataset.X[0].data(), mDataset.y.data(), mDataset.X.size());
		ImPlot::PlotLine("Model", line.data(), line.size());
		ImPlot::EndPlot();
	}

	mAppLog.Draw("Console");
}


/*

Homework Week 3:

- Build linear regression function
- Add interactivity
- Build a nice looking and useful app
- Drive information through user input in ImGui

*/