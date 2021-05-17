#ifndef INCLUDED_KLINK_JMATH_VECTOR2_H
#define INCLUDED_KLINK_JMATH_VECTOR2_H

#include "Common.h"
#include "Constants.h"

using namespace Klink::JMath::Constants;

namespace Klink::JMath
{

struct Vector2
{
	// Vars
	union
	{
		struct { float x, y; };
		std::array<float, 2> v;
	};


	// Constructors
	constexpr Vector2() noexcept : Vector2(0.0f) {}
	constexpr Vector2(float f) noexcept : x(f), y(f) {}
	constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}

	~Vector2()
	{}

	static Vector2 Zero() { return Vector2(); }
	static Vector2 One() { return Vector2(1.0f, 1.0f); }
	static Vector2 XAxis() { return Vector2(1.0f, 0.0f); }
	static Vector2 YAxis() { return Vector2(0.0f, 1.0f); }

	// Overloaded Operators
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}
	Vector2 operator*(const int& power) const
	{
		return Vector2(x * power, y * power);
	}
	Vector2 operator*(const float& power) const
	{
		return Vector2(x * power, y * power);
	}
	Vector2 operator/(const int& power) const
	{
		return Vector2(x / power, y / power);
	}
	Vector2 operator/(const float& power) const
	{
		return Vector2(x / power, y / power);
	}

	Vector2 &operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2 &operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vector2 &operator*=(const int& power)
	{
		x *= power;
		y *= power;
		return *this;
	}
	Vector2 &operator*=(const float& power)
	{
		x *= power;
		y *= power;
		return *this;
	}
	Vector2 &operator/=(int power)
	{
		x /= power;
		y /= power;
		return *this;
	}
	Vector2 &operator/=(float power)
	{
		x /= power;
		y /= power;
		return *this;
	}
	Vector2 &operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	bool operator==(const Vector2& other) const
	{
		return (fabs(x - other.x) < Constants::Epsilon && fabs(y - other.y) < Constants::Epsilon);
	}
	bool operator!=(const Vector2& other) const
	{
		return (x > other.x || x < other.x || y > other.y || y < other.y);
	}


	static inline Vector2 PerpendicularLH(const Vector2& v) { return Vector2(-v.y, v.x); }
	static inline Vector2 PerpendicularRH(const Vector2& v) { return Vector2(v.y, -v.x); }


	// Helpers
	inline float Magnitude() const
	{
		return (float)sqrt(x*x + y * y);
	}
	inline float MagnitudeSquared() const
	{
		return x * x + y * y;
	}
	inline void Normalize()
	{
		*this /= Magnitude();
	}
	inline Vector2 Normalized() const
	{
		return *this / Magnitude();
	}
	inline void Truncate(float magnitude)
	{
		if (this->Magnitude() > magnitude)
		{
			this->Normalize();
			this->x *= magnitude;
			this->y *= magnitude;
		}
	}

	static inline float Magnitude(const Vector2& v)
	{
		return v.Magnitude();
	}
	static inline float Dot(const Vector2& v1, const Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	static inline float Determinate(const Vector2& v0, const Vector2& v1)
	{
		return (v0.x * v1.y) - (v0.y * v1.x);
	}
	static inline float Distance(const Vector2& v1, const Vector2& v2)
	{
		return (sqrt(

			((v1.x - v2.x)*(v1.x - v2.x)) +
			((v1.y - v2.y)*(v1.y - v2.y))

		));
	}
	static inline float DistanceSquared(const Vector2& v1, const Vector2& v2)
	{
		return
		(	((v1.x - v2.x)*(v1.x - v2.x)) +
			((v1.y - v2.y)*(v1.y - v2.y)) );
	}
	static inline Vector2 RandomOnUnitCircle()
	{
		float x = static_cast<float>(rand());
		float y = static_cast<float>(rand());

		bool bX = rand() % 2;
		bool bY = rand() % 2;
		if (bX)
		{
			x = -x;
		}
		if (bY)
		{
			y = -y;
		}

		Vector2 r = { x, y };

		return r.Normalized();
	}
	static inline Vector2 Normalize(const Vector2& v)
	{
		return Vector2(v / Magnitude(v));
	}
	static inline void Truncate(Vector2& v, float magnitude)
	{
		v.Normalize();
		v *= magnitude;
	}
	static inline float Cross(const Vector2& v0, const Vector2& v1)
	{
		return (v0.x*v1.y) - (v0.y*v1.x);
	}
};

static inline Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)
{
	Vector2 l = v1 - v0;
	l *= t;
	return v0 + l;
}
static inline Vector2 QuadraticBezierCurveLerp(const Vector2& v0, const Vector2& v1, const Vector2& v2, const float t)
{
	float s = 1 - t;

	return (v0*(s*s)) + (v1 * 2 * (s*t)) + (v2*(t*t));
}
static inline Vector2 CubicBezierCurveLerp(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, const float t)
{
	float s = 1 - t;
	float sSqr = s * s;
	float tSqr = t * t;

	return (v0*sSqr*s) + (v1 * 3 * sSqr*t) + (v2 * 3 * s*tSqr) + (v3*tSqr*t);
}
static inline Vector2 CubicHermiteSplineLerp(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, const float t)
{
	float s = 1 - t;
	float tSqr = t * t;
	float sSqr = s * s;

	Vector2 U = (v1 - v0) * 3;
	Vector2 V = (v3 - v2) * 3;

	return v0*(1+2*t)*sSqr + v1*(1+2*s)*tSqr + U*sSqr*t - V*s*tSqr;
}
static inline Vector2 CatmullRomSplineLerp(const std::vector<Vector2> points, const float t)
{
	if (points.size() < 4)
		return Vector2();

	std::vector<Vector2> pointVelocities(points.size(), Vector2());	// Create a vector of velocities, all zero to start

	for (int i = 1; i < points.size() - 1; ++i)	// Loop through all points except beginning and end (which should remain 0)
	{
		pointVelocities[i] = (points[i + 1] - points[i - 1]) / 2.0f;
	}

	int splineT = static_cast<int>(floorf(t));
	float localT = t - static_cast<float>(splineT);

	Vector2 v0, v1, U, V;

	if (splineT < points.size()-1)
	{
		v0 = points[splineT];
		v1 = points[splineT + 1];

		U = pointVelocities[splineT];
		V = pointVelocities[splineT + 1];
	}
	else
	{
		v0 = points[splineT];
		v1 = Vector2::Zero();

		U = pointVelocities[splineT];
		V = pointVelocities[splineT];
	}

	float s = 1 - localT;
	float tSqr = localT * localT;
	float sSqr = s * s;

	return v0 * (1 + 2 * localT)*sSqr + v1 * (1 + 2 * s)*tSqr + U * sSqr*localT - V * s*tSqr;
}
static inline Vector2 ProjectAonB(const Vector2& a, const Vector2& b)
{
	// proj A->B = ( A.B / ||B||^2 ) * B
	return b * (Vector2::Dot(a, b) / b.MagnitudeSquared());
}
} // namespace Klink::JMath

#endif // INCLUDED_KLINK_JMATH_VECTOR2_H