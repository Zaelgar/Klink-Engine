//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#pragma once

namespace Siege::ML
{
	class Neuron;
	using Layer = std::vector<Neuron>;

	struct Connection
	{
		float weight;
	};

	class Neuron
	{
	public:
		Neuron(size_t numOutputs, size_t myIndex);

		void SetOutputValue(float value) { mOutputValue = value; }
		float GetOutputValue() const { return mOutputValue; }

		void FeedForward(const Layer& previousLayer);
		void CalculateOutputGradients(float targetValue);
		void CalculateHiddenGradients(const Layer& nextLayer);
		void UpdateInputWeights(Layer& previousLayer);

	private:
		std::vector<Connection> mOutputWeights;
		size_t mMyIndex = 0;
		float mOutputValue = 0.0f;
		float mGradient = 0.0f;
	};

	class NeuralNetwork
	{
	public:
		NeuralNetwork(const std::vector<size_t>& topology);

		void FeedFoward(const std::vector<float>& inputValues);
		void BackPropagate(const std::vector<float>& targetValues);
		std::vector<float> GetResults() const;

	private:
		std::vector<Layer> mLayers;
	};
}


// XOR - 2, 2, 1
// Setup training data
// 0, 0 -> 0
// 0, 1 -> 1
// 1, 0 -> 1
// 1, 1 -> 0

// training index: 0, 1, 0, 2, 2, 1, 3, 0, 1, 2 ....

// https://archive.ics.uci.edu/ml/datasets/Optical+Recognition+of+Handwritten+Digits