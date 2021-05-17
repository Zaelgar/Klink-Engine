#ifndef INCLUDED_ML_LINEAR_REGRESSION_H
#define INCLUDED_ML_LINEAR_REGRESSION_H

#include "Datasets.h"

namespace Klink::ML
{
	class LinearRegression
	{
	public:
		float LearnToFit(const Dataset& dataset);
		float SimpleFit(const Dataset& dataset);
		float Predict(float X) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float learningRate = 0.0001f;
	};
}

#endif // INCLUDED_ML_LINEAR_REGRESSION_H