#ifndef INCLUDED_KLINK_JMATH_RANDOM_H
#define INCLUDED_KLINK_JMATH_RANDOM_H

#include "Common.h"

namespace Klink::JMath::Random
{
	static std::random_device sRandomDevice{};
	static std::mt19937 sRandomEngine{ sRandomDevice() };

	static float RandomFloatNormal(float mean, float stddev)
	{
		std::normal_distribution<float> distribution( mean, stddev );
		
		return distribution(sRandomEngine);
	}

	static float RandomFloatUniform(float min = 0.0f, float max = 1.0f)
	{
		std::uniform_real_distribution<float> distribution(min, max);

		return distribution(sRandomEngine);
	}

	static int RandomIntUniform(int min = INT_MIN, int max = INT_MAX)
	{
		std::uniform_int_distribution<int> distribution(min, max);

		return distribution(sRandomEngine);
	}
}

#endif // INCLUDED_KLINK_JMATH_RANDOM_H