#include "GameState.h"

#include <ImGui/Inc/ImPlot.h>

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::ML;
using namespace Klink::Physics;

void GameState::Initialize()
{
	mLogReg.learningRate = 0.00001f;

	GenerateData();
	Train();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{

}

void GameState::DebugUI()
{
	if (ImPlot::BeginPlot("Logistic Regression", "x0", "x1"))
	{
		const float m = -mLogReg.b1 / mLogReg.b2;
		const float b = -mLogReg.b0 / mLogReg.b2;
		const float startX = mMinX;
		const float endX = mMaxX;
		const float startY = startX * m + b;
		const float endY = endX * m + b;
		std::vector<ImVec2> line;
		line.push_back({ startX, startY });
		line.push_back({ endX, endY });
		ImPlot::PlotLine("Model", line.data(), line.size());

		std::vector<float> ax, ay, bx, by;
		for (int i = 0; i < mDataset.X[0].size(); ++i)
		{
			float p = mLogReg.b0 + mLogReg.b1 * mDataset.X[0][i] + mLogReg.b2 * mDataset.X[1][i];
			if (p >= 0)
			{
				ax.push_back(mDataset.X[0][i]);
				ay.push_back(mDataset.X[1][i]);
			}
			else
			{
				bx.push_back(mDataset.X[0][i]);
				by.push_back(mDataset.X[1][i]);
			}
		}
		if (!ax.empty())
			ImPlot::PlotScatter("Ones", ax.data(), ay.data(), (int)ax.size());
		if (!bx.empty())
			ImPlot::PlotScatter("Zeros", bx.data(), by.data(), (int)bx.size());
		ImPlot::EndPlot();
	}

	mAppLog.Draw("Console");

	ImGui::DragInt("Sample Count", &mSampleCount);
	ImGui::DragFloat("Target B0", &mTargetB0);
	ImGui::DragFloat("Target B1", &mTargetB1);
	ImGui::DragFloat("Target B2", &mTargetB2);
	ImGui::DragFloat("Min X", &mMinX);
	ImGui::DragFloat("Max X", &mMaxX);
	ImGui::DragFloat("Noise", &mNoise);
	ImGui::DragFloat("Learning Rate", &mLogReg.learningRate, 0.000001f, 0.0f, FLT_MAX, "%.7f");
	if (ImGui::Button("Generate"))
		GenerateData();

	ImGui::DragInt("Iteration", &mIteration);
	if (ImGui::Button("Train"))
		Train();
}

void GameState::GenerateData()
{
	mAppLog.AddLog("Generating data ...\n");
	mDataset = Datasets::MakeLogistic(mSampleCount, mTargetB0, mTargetB1, mTargetB2, mMinX, mMaxX, mNoise);
}

void GameState::Train()
{
	for (int i = 0; i < mIteration; ++i)
	{
		float error = mLogReg.Fit(mDataset);
		mAppLog.AddLog("b0 = %f, b1 = %f, b2 = %f, error = %f\n", mLogReg.b0, mLogReg.b1, mLogReg.b2, error);
	}
}
