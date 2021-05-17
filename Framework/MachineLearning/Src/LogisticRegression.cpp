#include "Precompiled.h"
#include "LogisticRegression.h"

#include "Datasets.h"

using namespace Klink::ML;

float LogisticRegression::Fit(const Dataset& dataset)
{
	float predictB0 = 0.f;
	float predictB1 = 0.f;
	float predictB2 = 0.f;
	float totalCost = 0.f;
	float size = static_cast<int>(dataset.X[0].size());

	for (int i = 0; i < dataset.properties.numSamples; ++i)
	{
		const float cost = (-dataset.y[i] * log(Predict(dataset.X[0][i], dataset.X[1][i]))) - ((1 - dataset.y[i]) * log(1 - Predict(dataset.X[0][i], dataset.X[1][i])));
		predictB0 = (1.0f * (Predict(dataset.X[0][i], dataset.X[1][i]) - dataset.y[i]));
		predictB1 = (dataset.X[0][i] * (Predict(dataset.X[0][i], dataset.X[1][i]) - dataset.y[i]));
		predictB2 = (dataset.X[1][i] * (Predict(dataset.X[0][i], dataset.X[1][i]) - dataset.y[i]));
		b0 -= learningRate * predictB0;
		b1 -= learningRate * predictB1;
		b2 -= learningRate * predictB2;
		totalCost += cost;
	}
	return totalCost / size;
}

float LogisticRegression::Predict(float X0, float X1) const
{
	const float z = b0 + (b1 * X0) + (b2 * X1);
	return 1.0f / (1.0f + exp(-z));
}