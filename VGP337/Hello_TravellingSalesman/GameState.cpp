#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;
using namespace Klink::ML;
using namespace Klink::Physics;

namespace
{
	struct MatingFunction
	{
		MatingFunction(float mutationRate, float crossoverRate, int maxGeneValue)
			: mMutationRate(mutationRate)
			, mCrossoverRate(crossoverRate)
			, mMaxGeneValue(maxGeneValue)
		{}

		const float mMutationRate = 0.1f;
		const float mCrossoverRate = 0.45f;
		const int mMaxGeneValue = 1;
		const int mMinGeneValue = 0;

		GeneticAlgorithm::Genome operator()
			(
				const GeneticAlgorithm::Genome& parent1,
				const GeneticAlgorithm::Genome& parent2
				) const
		{
			// RVO - Return value optimization
			GeneticAlgorithm::Genome offspring;

			offspring.chromosome = parent1.chromosome;

			// MUTATION RATE Swap two indices
			const int mutations = ceil(offspring.chromosome.size() * mMutationRate);
			for (int i = 0; i < mutations; ++i)
			{
				const int one = Random::RandomIntUniform(0, offspring.chromosome.size() - 1);
				//const int two = (one + 1) % offspring.chromosome.size();
				const int two = offspring.chromosome.size() - 1;
				std::swap(offspring.chromosome[one], offspring.chromosome[two]);
			}

			const int crossovers = ceil(offspring.chromosome.size() * mCrossoverRate);
			for (int i = 0; i < crossovers; ++i)
			{
				const int crossIndex = Random::RandomIntUniform(0, offspring.chromosome.size() - 1);

				const int switchValue = offspring.chromosome[crossIndex];
				const int p2switchValue = parent2.chromosome[crossIndex];

				offspring.chromosome[crossIndex] = p2switchValue;
				for (int j = 0; j < offspring.chromosome.size(); ++j)
				{
					if (j == crossIndex)
						continue;
					if (offspring.chromosome[j] == p2switchValue)
						offspring.chromosome[j] = switchValue;
				}
			}

			return offspring;
		}
	};

	struct FitnessFunctor
	{
		FitnessFunctor(const std::vector<Vector2>& locations)
			: mLocations(locations)
		{}

		const std::vector<Vector2>& mLocations;

		float operator()(const GeneticAlgorithm::Genome& genome) const
		{
			// Find the distance from each town to the next using a genome's chromosomes as city indices
			float fitness = 0.0f;

			for (size_t i = 0; i + 1 < genome.chromosome.size(); ++i)
			{
				fitness += Vector2::Distance(mLocations[genome.chromosome[i]], mLocations[genome.chromosome[i + 1]]);
			}

			return fitness;
		}
	};
}


void GameState::Initialize()
{
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	if (mIsInitialized && !mIsFinished)
	{
		auto& best = mGeneticAlgorithm.GetBestGenome();
		if (mIsUsingIterations)
		{
			//for (int i = 0; i < mIterations; ++i)
			if (mCurrentIteration < mIterations)
			{
				mCurrentIteration++;
				mGeneticAlgorithm.Advance();
				std::string bestFit;
				auto bestGenome = mGeneticAlgorithm.GetBestGenome();
				for (auto& gene : bestGenome.chromosome)
				{
					std::string format = " ";
					format += std::to_string(gene);
					bestFit += format;
				}
				mAppLog.AddLog("F = %f in Generation %d: %s\n", mFitnessThreshold - bestGenome.fitness, mGeneticAlgorithm.GetGeneration(), bestFit.c_str());
			}
			mIsFinished = (mCurrentIteration >= mIterations);
		}
		else if (best.fitness > mFitnessThreshold)
		{
			mGeneticAlgorithm.Advance();
			std::string bestFit;
			auto bestGenome = mGeneticAlgorithm.GetBestGenome();
			for (auto& gene : bestGenome.chromosome)
			{
				std::string format = " ";
				format += std::to_string(gene);
				bestFit += format;
			}
			mAppLog.AddLog("F = %f in Generation %d: %s\n", mFitnessThreshold - bestGenome.fitness, mGeneticAlgorithm.GetGeneration(), bestFit.c_str());
			mIsFinished = (mCurrentIteration >= mIterations);
		}
		else
		{
			mIsFinished = true;
		}
	}
}

void GameState::Render()
{
	// If the program is finished running, draw the lines to connect the cities
	if (mIsFinished)
	{
		auto& genome = mGeneticAlgorithm.GetBestGenome();

		float percent = 1.0f / genome.chromosome.size();
		//float step = 255.0f * percent;

		for (int i = 0; i < genome.chromosome.size() - 1; ++i)
		{
			int genomeIndex = genome.chromosome[i];
			int genomeIndex2 = genome.chromosome[i + 1];
			Colours::Colour col = { percent * i, 0.1f, 0.1f };
			SimpleDraw::DrawScreenLine(mLocations[genomeIndex], mLocations[genomeIndex2], col);
		}
	}

	for (int i = 0; i < mLocations.size(); ++i)
	{
		SimpleDraw::DrawScreenCircle(mLocations[i], i * 5 + 2.0f, mCityColour);
	}

	SimpleDraw::DrawScreenRect({ minX, minY }, { maxX, maxY }, Colours::Green);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	const auto gs = GraphicsSystem::Get();
	const auto screenWdith = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();

	if (ImGui::CollapsingHeader("City Generation Settings"))
	{
		ImGui::DragInt("Number Of Cities", &mNumLocations, 1.0f, 4, 20);
		ImGui::DragFloat("City Draw Radius", &mCityRadius, 1.0f, 3.0f, 30.0f);
		ImGui::DragFloat("Minimum X", &minX, 1.0f, mCityRadius, screenWdith - mCityRadius * 3);
		ImGui::DragFloat("Minimum Y", &minY, 1.0f, mCityRadius, screenHeight - mCityRadius * 3);
		ImGui::DragFloat("Maximum X", &maxX, 1.0f, mCityRadius, screenWdith - mCityRadius);
		ImGui::DragFloat("Maximum Y", &maxY, 1.0f, mCityRadius, screenHeight - mCityRadius);
		ImGui::ColorEdit3("City Render Colour", &mCityColour.r);
	}
	if (ImGui::CollapsingHeader("Genetic Algorithm Settings"))
	{
		ImGui::DragInt("Population Count", &mPopulation, 1.0f, 5, 500);
		ImGui::DragFloat("Crossover Rate", &mCrossOverRate, 0.025f, 0.1f, 0.9f);
		ImGui::DragFloat("Mutation Rate", &mMutationRate, 0.025f, 0.025f, 0.9f);
	}

	if (ImGui::Button("Initialize New Sequence And Run!"))
	{
		mAppLog.AddLog("Initializing new population and creating first generation ... \n");

		mLocations.clear();
		mLocations.resize(mNumLocations);
		std::vector<GeneticAlgorithm::Genome> firstGeneration;
		float thetaStep = TwoPi / mNumLocations;
		float theta = 0.0f;
		for (auto& location : mLocations) // TODO range based for loops V
		{

			float x = mCircleRadius * cos(theta) + mCircleCenter.x;
			float y = mCircleRadius * sin(theta) + mCircleCenter.y;
			location = { x, y };
			theta += thetaStep;
		}

		mFitnessThreshold = 0.f;
		for (int i = 0; i+1 < mNumLocations; ++i)
		{
			mFitnessThreshold += Vector2::Distance(mLocations[i], mLocations[i + 1]);
		}

		firstGeneration.resize(mPopulation);
		for (auto& genome : firstGeneration)
		{
			// Reserve total population count for the first generation
			genome.chromosome.resize(mNumLocations);

			// Simple initialization from [0..mNumLocations) to avoid duplication. Mate function should handle keeping the structure valid
			std::iota(genome.chromosome.begin(), genome.chromosome.end(), 0);
			for (auto num : genome.chromosome)
			{
				int index1 = Random::RandomIntUniform(0, genome.chromosome.size() - 1);
				int index2 = Random::RandomIntUniform(0, genome.chromosome.size() - 1);
				if (index1 == index2)
					index2 = (index2 + 1) % genome.chromosome.size();
				std::swap(genome.chromosome[index1], genome.chromosome[index2]);
			}
		}

		mGeneticAlgorithm.Initialize
		(
			mPopulation,
			mNumLocations,
			mNumLocations - 1,
			mCrossOverRate,
			mMutationRate,
			FitnessFunctor(mLocations),
			MatingFunction(mMutationRate, mCrossOverRate, mNumLocations - 1),
			std::move(firstGeneration)
		);

		mIsInitialized = true;
		mIsFinished = false;
		mCurrentIteration = 0;
	}

	mAppLog.Draw("Console");
	ImGui::End();
}