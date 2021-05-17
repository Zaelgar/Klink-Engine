#ifndef INCLUDED_KLINK_MATH_COMMON_H
#define INCLUDED_KLINK_MATH_COMMON_H

// engine h
#include <Core/Inc/Core.h>

// standard
#include <cmath>
#include <numeric>
#include <random>

namespace Klink::JMath
{
	static float Abs(float value) { return (value >= 0.0f) ? value : -value; }
}

#endif // INCLUDED_JMATH_H