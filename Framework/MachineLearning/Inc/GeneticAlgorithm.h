//====================================================================================================
// KLINK - When you just want things to KLINK!
// Copyright (c) 2020 Jaidon van Herwaarden. All rights reserved.
//====================================================================================================

#pragma once

namespace Klink::ML
{
	class GeneticAlgorithm
	{
	public:
		// Genome is the set of chromosomes that represent a particular creature
		// Genome -> chromosomes -> genes
		struct Genome
		{
			std::vector<int> chromosome; // [gene][gene][gene][gene]...
			float fitness = 0.0f;
		};

		using ComputeFitnessFunc = std::function<float(const Genome&)>;
		using MateFunc = std::function<Genome(const Genome& p1, const Genome& p2)>;

		void Initialize(int population, int chromoLength, int maxGeneValue, float crossoverRate, float mutationRate, ComputeFitnessFunc computeFitness, MateFunc mateFunction, std::vector<Genome> firstGeneration = {});
		void Advance();

		const Genome& GetBestGenome() const { return mPopulation.front(); };
		int GetGeneration() const { return mGeneration; }

	private:
		void EvaluatePopulation();
		Genome Mate(const Genome& parent1, const Genome& parent2);

		ComputeFitnessFunc mComputeFitness;
		MateFunc mMate;

		std::vector<Genome> mPopulation;
		int mGeneration = 0;
		int mMaxGeneValue = 0;
		
		float mCrossoverRate = 0.45f; // percentages
		float mMutationRate = 0.1f;
	};
}