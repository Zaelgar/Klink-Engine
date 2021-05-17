#pragma once

namespace Klink::ML
{
	struct DatasetProperties
	{
		void Initialize(size_t _featureCount, size_t _numSamples, float _minX, float _maxX, float _noise, int _bSize)
		{
			featureCount = _featureCount;
			numSamples = _numSamples;
			minX = _minX;
			maxX = _maxX;
			noise = _noise;
			b.resize(_bSize);
		}

		size_t featureCount = 1;
		size_t numSamples = 2;
		float minX = -1;
		float maxX = 1;
		float noise = 0;
		std::vector<float> b;
	};

	struct Dataset
	{
		Dataset() = default;
		Dataset(DatasetProperties props)
		{
			properties = props;
			y.reserve(properties.numSamples);
			X.resize(properties.featureCount);
			for (auto& featureSet : X)
			{
				featureSet.reserve(properties.numSamples);
			}
		}

		DatasetProperties properties;
		std::vector<std::vector<float>> X;
		std::vector<float> y;
	};

	namespace Datasets
	{
		Dataset MakeLinear(size_t samples, float b0, float b1, float minX, float maxX, float noise);
		Dataset MakeLinear(const DatasetProperties& props);
		Dataset MakeLogistic(size_t samples, float b0, float b1, float b2, float minX, float maxX, float noise);
		Dataset MakeLogistic(const DatasetProperties& props);
	}
}