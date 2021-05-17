//====================================================================================================
// KLINK - When you just want things to KLINK!
// Copyright (c) 2020 Jaidon van Herwaarden. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using namespace Klink::ML;
using namespace Klink::JMath::Random;

void GeneticAlgorithm::Initialize(int population, int chromoLength, int maxGeneValue, float crossoverRate, float mutationRate, ComputeFitnessFunc computeFitness, MateFunc mateFunction, std::vector<Genome> firstGeneration)
{
	mComputeFitness = std::move(computeFitness);
	mMate = std::move(mateFunction);

	mGeneration = 0;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;
	mMaxGeneValue = maxGeneValue;
	mPopulation = std::move(firstGeneration);

	if (mPopulation.empty())
	{
		// Create initial population
		mPopulation.resize(population);
		for (auto& genome : mPopulation)
		{
			genome.chromosome.reserve(chromoLength);
			for (int i = 0; i < chromoLength; ++i)
				genome.chromosome.push_back(RandomIntUniform(0, maxGeneValue));
		}
	}
	EvaluatePopulation();
}

void GeneticAlgorithm::Advance()
{
	++mGeneration;

	std::vector<Genome> newPopulation;
	newPopulation.reserve(mPopulation.size());

	// Perform Elitism: keep top %, mate the rest using top 50%.
	// Another way is roulette. Split a roulette wheel into weighted categories based on fitness and randomly select from the wheel.
	//   this way, you get more of the better fitted parents but still retain the randomness of the whole spectrum.
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
		newPopulation.push_back(mPopulation[i]);

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[RandomIntUniform(0, cutoff)];
		const auto& parent2 = mPopulation[RandomIntUniform(0, cutoff)];
		newPopulation.push_back(mMate(parent1, parent2));
	}
	mPopulation = std::move(newPopulation);
	EvaluatePopulation();
}

void GeneticAlgorithm::EvaluatePopulation()
{
	for (auto& genome : mPopulation)
		genome.fitness = mComputeFitness(genome);
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b) { return a.fitness < b.fitness; });
}

GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	Genome offspring;
	offspring.chromosome.reserve(parent1.chromosome.size());

	for (size_t i = 0; i < parent1.chromosome.size(); ++i)
	{
		if (RandomFloatUniform() < mMutationRate)
			offspring.chromosome.push_back(RandomIntUniform(0, mMaxGeneValue));
		else if (RandomFloatUniform() < mCrossoverRate)
			offspring.chromosome.push_back(parent1.chromosome[i]);
		else
			offspring.chromosome.push_back(parent2.chromosome[i]);
	}

	return offspring;
}