//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "Datasets.h"

using namespace Siege::ML;

namespace
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandomEngine{ sRandomDevice() };

	float RandomNormal(float mean, float stdev)
	{
		return std::normal_distribution<float>{ mean, stdev }(sRandomEngine);
	}

	float RandomUniform(float min, float max)
	{
		return std::uniform_real_distribution<float>{ min, max }(sRandomEngine);
	}
}

Dataset Datasets::MakeLinear(size_t samples, float b0, float b1, float minX, float maxX, float noise)
{
	Dataset dataset;
	dataset.x0.reserve(samples);
	dataset.y.reserve(samples);

	for (size_t i = 0; i < samples; ++i)
	{
		const float x = RandomUniform(minX, maxX);
		const float y = b0 + (b1 * x) + RandomNormal(0.0f, noise);
		dataset.x0.push_back(x);
		dataset.y.push_back(y);
	}

	return dataset;
}

Dataset Datasets::MakeLogistic(size_t samples, float b0, float b1, float b2, float minX, float maxX, float noise)
{
	Dataset dataset;
	dataset.x0.reserve(samples);
	dataset.x1.reserve(samples);
	dataset.y.reserve(samples);

	const float m = -b1 / b2;
	const float b = -b0 / b2;

	for (size_t i = 0; i < samples; ++i)
	{
		const float x0 = RandomUniform(minX, maxX);
		const float point = (m * x0) + b;
		const float delta = RandomNormal(0.0f, noise);
		const float x1 = point + delta;
		const float y = (delta > 0.0f) ? 1.0f : 0.0f;
		dataset.x0.push_back(x0);
		dataset.x1.push_back(x1);
		dataset.y.push_back(y);
	}

	return dataset;
}