#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::ML;
using namespace Klink::Physics;

namespace
{
	const std::string kTarget = "Klink engine is the next big thing!";
	const int kValidGeneValue = 126 - 32;
}

void GameState::Initialize()
{

}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	if (mIsInitialized)
	{
		auto& best = mGeneticAlgorithm.GetBestGenome();
		if (best.fitness < static_cast<int>(kTarget.size()))
		{
			mGeneticAlgorithm.Advance();
			std::string bestStr;
			for (auto& gene : mGeneticAlgorithm.GetBestGenome().chromosome)
			{
				bestStr += static_cast<char>(gene + 32);
			}
			mAppLog.AddLog("Generation %d: %s\n", mGeneticAlgorithm.GetGeneration(), bestStr.c_str());
		}
	}
}

void GameState::Render()
{
}

void GameState::DebugUI()
{
	ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	

	if (ImGui::Button("Run!"))
	{
		mAppLog.AddLog("Initializing new population ... \n");
		mGeneticAlgorithm.Initialize
		(
			mPopulationCount,
			static_cast<int>(kTarget.size()),
			kValidGeneValue,
			mCrossOverRate,
			mMutationRate,
			[](auto& genome)
		{
			float fitness = 0.0f;
			for (size_t i = 0; i < kTarget.size(); ++i)
			{
				if (kTarget[i] == genome.chromosome[i] + 32)
					fitness += 1.0f;
			}
			return fitness;
		}
		);
		mIsInitialized = true;
	}

	mAppLog.Draw("Console");
	ImGui::End();
}

// For homework,
// 1) follow geeks for geeks example to test GA
// 2) allow crossover/mutate to be std::functions and use GA for pathfinding


/*

ImVec2 offset{ 0.0f, 20.0f };
	ImGui::SetNextWindowSize(ImVec2{ 128.0f, 128.0f } + offset);
	ImGui::Begin("Target", nullptr, ImGuiWindowFlags_NoResize);
	ImVec2 winPos = ImGui::GetWindowPos();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			int i = x + (y * 8);
			ImVec2 min{ x * 16.0f, y * 16.0f };
			ImVec2 max = min + ImVec2{ 16.0f, 16.0f };
			float intensity = mDataset.inputs[mCurrentEntry][i];
			ImColor color{ intensity, intensity, intensity };
			drawList->AddRectFilled(min + winPos + offset, max + winPos + offset, color);
		}
	}
	ImGui::End();





	// This works better for XOR
	double Tanh(double x)
	{
		return tanh(x);
	}

	double TanhDerivative(double x)
	{
		return 1 - (x * x);
	}

	// This works better for digits
	double Sigmoid(double x)
	{
		return 1 / (1 + exp(-x));
	}

	double SigmoidDerivative(double x)
	{
		double out = Sigmoid(x);
		return out * (1 - out);
	}



*/