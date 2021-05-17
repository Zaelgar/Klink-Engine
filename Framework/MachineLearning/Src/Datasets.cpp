#include "Precompiled.h"
#include "Datasets.h"

using namespace Klink::ML;
using namespace Klink::JMath::Random;

Dataset Datasets::MakeLinear(size_t samples, float b0, float b1, float minX, float maxX, float noise)
{
	DatasetProperties props;
	props.Initialize(1, samples, minX, maxX, noise, 2);

	Dataset dataset(props);
	dataset.properties.Initialize(1, samples, minX, maxX, noise, 1);

	dataset.X.resize(samples);
	dataset.y.resize(samples);

	for (size_t i = 0; i < samples; ++i)
	{
		dataset.X[0][i] = JMath::Random::RandomFloatUniform(minX, maxX);
		dataset.y[i] = b0 + (b1 * dataset.X[0][i]) + JMath::Random::RandomFloatNormal(0.0f, noise);
	}

	return dataset;
}

Dataset Datasets::MakeLinear(const DatasetProperties& props)
{
	Dataset dataset(props);

	for (size_t i = 0; i < props.numSamples; ++i)
	{
		for (size_t j = 0; j < props.featureCount; ++j)
		{
			dataset.X[j][i] = JMath::Random::RandomFloatUniform(dataset.properties.minX, dataset.properties.maxX);
		}
		dataset.y[i] = props.b[0] + (props.b[1] * dataset.X[0][i]) + JMath::Random::RandomFloatNormal(0.0f, props.noise);
	}

	return dataset;
}

Dataset Datasets::MakeLogistic(size_t samples, float b0, float b1, float b2, float minX, float maxX, float noise)
{
	DatasetProperties props;
	props.Initialize(2, samples, minX, maxX, noise, 3);

	Dataset dataset(props);

	const float m = -b1 / b2;
	const float b = -b0 / b2;

	for (size_t i = 0; i < samples; ++i)
	{
		const float x0 = RandomFloatUniform(minX, maxX);
		const float point = (m * x0) + b;
		const float delta = RandomFloatNormal(0.0f, noise);
		const float x1 = point + delta;
		const float y = (delta > 0.0f) ? 1.0f : 0.0f;

		dataset.X[0].push_back(x0);
		dataset.X[1].push_back(x1);
		dataset.y.push_back(y);
	}

	return dataset;
}

Dataset Datasets::MakeLogistic(const DatasetProperties& props)
{
	Dataset dataset(props);

	dataset.X[0].reserve(props.numSamples);
	dataset.X[1].reserve(props.numSamples);
	dataset.y.reserve(props.numSamples);

	const float m = -props.b[1] / props.b[2];
	const float b = -props.b[0] / props.b[2];

	for (size_t i = 0; i < props.numSamples; ++i)
	{
		const float x0 = RandomFloatUniform(props.minX, props.maxX);
		const float point = (m * x0) + b;
		const float delta = RandomFloatNormal(0.0f, props.noise);
		const float x1 = point + delta;
		const float y = (delta > 0.0f) ? 1.0f : 0.0f;

		dataset.X[0].push_back(x0);
		dataset.X[1].push_back(x1);
		dataset.y.push_back(y);
	}

	return dataset;
}