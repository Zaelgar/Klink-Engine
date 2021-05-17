#include "Precompiled.h"
#include "LinearRegression.h"

#include "Datasets.h"

using namespace Klink::ML;

float LinearRegression::SimpleFit(const Dataset& dataset)
{
	int size = dataset.X.size();

	float meanX = 0.0f;
	float meany = 0.0f;
	float crossDevXy = 0.0f;
	float crossDevXX = 0.0f;
	for (int i = 0; i < size; ++i)
	{
		meanX += dataset.X[0][i];
		meany += dataset.y[i];
		crossDevXy += dataset.X[0][i] * dataset.y[i];
		crossDevXX += dataset.X[0][i] * dataset.X[0][i];
	}

	meanX /= static_cast<float>(size);
	meany /= static_cast<float>(size);

	crossDevXy -= (size * meany * meanX);
	crossDevXX -= (size * meanX * meanX);

	b1 = crossDevXy / crossDevXX;
	b0 = meany - (b1 * meanX);

	float predict = 0.0f;
	float difference = 0.0f;
	float error = 0.0f;
	for (int i = 0; i < size; ++i)
	{
		predict = Predict(dataset.X[0][i]);
		difference = predict - dataset.y[i];
		difference *= difference;
		error += difference;
	}

	error /= static_cast<float>(size);

	return error;
}

float LinearRegression::LearnToFit(const Dataset& dataset)
{
	int size = dataset.X.size();

	float dmSum = 0.0f;
	float dcSum = 0.0f;
	float n = (-2.0f / static_cast<float>(size));
	float predictionY = 0.0f;
	for (int i = 0; i < size; ++i)
	{
		predictionY = Predict(dataset.X[0][i]);
		dmSum += (n * dataset.X[0][i] * (dataset.y[i] - predictionY));
		dcSum += (n * (dataset.y[i] - predictionY));
	}

	b0 = b0 - learningRate * dcSum;
	b1 = b1 - learningRate * dmSum;

	float predict = 0.0f;
	float difference = 0.0f;
	float error = 0.0f;
	for (int i = 0; i < size; ++i)
	{
		predict = Predict(dataset.X[0][i]);
		difference = predict - dataset.y[i];
		difference *= difference;
		error += difference;
	}

	error /= static_cast<float>(size);

	return error;
}

float LinearRegression::Predict(float X) const
{
	return b0 + (b1 * X);
}