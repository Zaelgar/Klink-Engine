#ifndef INCLUDED_ML_LOGISTIC_REGRESSION_H
#define INCLUDED_ML_LOGISTIC_REGRESSION_H

#include "Datasets.h"

namespace Klink::ML
{
	class LogisticRegression
	{
	public:
		float Fit(const Dataset& dataset);
		float Predict(float X0, float X1) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float b2 = 0.0f;
		float learningRate = 0.0001f;
	};
}

#endif // INCLUDED_ML_LOGISTIC_REGRESSION_H