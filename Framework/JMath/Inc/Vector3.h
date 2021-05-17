#ifndef INCLUDED_KLINK_JMATH_VECTOR3_H
#define INCLUDED_KLINK_JMATH_VECTOR3_H

#include "Common.h"
#include "Constants.h"

using namespace Klink::JMath::Constants;

namespace Klink::JMath
{

struct Vector3
{
	// Vars
	union
	{
		struct { float x, y, z; };
		std::array<float, 3> v;
	};

	// Constructors
	Vector3() noexcept : Vector3(0.0f) {}
	Vector3(float f) noexcept : x(f), y(f), z(f) {}
	Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

	~Vector3()
	{}

	static Vector3 Zero() { return Vector3(); }
	static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
	static Vector3 XAxis() { return Vector3(1.0f, 0.0f, 0.0f); }
	static Vector3 YAxis() { return Vector3(0.0f, 1.0f, 0.0f); }
	static Vector3 ZAxis() { return Vector3(0.0f, 0.0f, 1.0f); }

	// Overloaded Operators
	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}
	Vector3 operator*(const int& power) const
	{
		return Vector3(x * power, y * power, z * power);
	}
	Vector3 operator*(const float& power) const
	{
		return Vector3(x * power, y * power, z * power);
	}
	Vector3 operator/(const int& power) const
	{
		return Vector3(x / power, y / power, z / power);
	}
	Vector3 operator/(const float& power) const
	{
		return Vector3(x / power, y / power, z / power);
	}

	Vector3 &operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Vector3 &operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	Vector3 &operator*=(const int& power)
	{
		x *= power;
		y *= power;
		z *= power;
		return *this;
	}
	Vector3 &operator*=(const float& power)
	{
		x *= power;
		y *= power;
		z *= power;
		return *this;
	}
	Vector3 &operator/=(int power)
	{
		x /= power;
		y /= power;
		z /= power;
		return *this;
	}
	Vector3 &operator/=(float power)
	{
		x /= power;
		y /= power;
		z /= power;
		return *this;
	}
	Vector3 &operator=(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool operator==(const Vector3& other) const
	{
		return (fabs(x - other.x) < Epsilon && fabs(y - other.y) < Epsilon && fabs(z - other.z) < Epsilon);
	}
	bool operator!=(const Vector3& other) const
	{
		return (x > other.x || x < other.x || y > other.y || y < other.y || z > other.z || z < other.z);
	}

	// Functions and Methods
	inline float Magnitude() const
	{
		return (float)sqrt(x*x + y * y + z * z);
	}
	inline float MagnitudeSquared() const
	{
		return x * x + y * y + z * z;
	}
	inline void Normalize()
	{
		*this /= Magnitude();
	}
	inline Vector3 Normalized() const
	{
		return *this / Magnitude();
	}
	inline float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3((v2.y * v1.z - v2.z * v1.y), (v2.z * v1.x - v2.x * v1.z), (v2.x * v1.y - v2.y * v1.x));
	}
};

static inline float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
static inline float Magnitude(const Vector3& v)
{
	return (float)sqrt(v.x*v.x + v.y * v.y + v.z * v.z);
}
static inline float MagnitudeSquared(Vector3 &v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
static inline Vector3 Normalize(const Vector3& v)
{
	return Vector3(v / Magnitude(v));
}
static inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3((v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x));
}
static inline float Distance(const Vector3& v1, const Vector3& v2)
{
	return (sqrt(

		((v1.x - v2.x)*(v1.x - v2.x)) +
		((v1.y - v2.y)*(v1.y - v2.y)) +
		((v1.z - v2.z)*(v1.z - v2.z))

	));
}
static inline Vector3 RandomOnUnitSphere()
{	
	int x = rand();
	if (x % 2 == 0)
		x *= -1;
	int y = rand();
	if (y % 2 == 0)
		y *= -1;
	int z = rand();
	if (z % 2 == 0)
		z *= -1;

	Vector3 r = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};

	return r.Normalized();
}
static inline Vector2 RandomOnUnitCircle()
{
	int x = rand();
	if (x % 2 == 0)
		x *= -1;
	int y = rand();
	if (y % 2 == 0)
		y *= -1;

	Vector2 r = { static_cast<float>(x), static_cast<float>(y) };

	return r.Normalized();
}
static inline Vector3 Truncate(Vector3& v, float magnitude)
{
	v.Normalize();
	v *= magnitude;
	return v;
}
static inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
{
	Vector3 l = v1 - v0;
	l *= t;
	return v0 + l;
}
static inline Vector3 QuadraticBezierCurveLerp(const Vector3& v0, const Vector3& v1, const Vector3& v2, const float t)
{
	float s = 1 - t;

	return (v0*(s*s)) + (v1 * 2 * (s*t)) + (v2*(t*t));
}
static inline Vector3 CubicBezierCurveLerp(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, float t)
{
	float s = 1 - t;
	float sSqr = s * s;
	float tSqr = t * t;

	return (v0*sSqr*s) + (v1 * 3 * sSqr*t) + (v2 * 3 * s*tSqr) + (v3*tSqr*t);
}
static inline Vector3 CubicHermiteSplineLerp(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const float t)
{
	float s = 1 - t;
	float tSqr = t * t;
	float sSqr = s * s;

	Vector3 U = (v1 - v0) * 3;
	Vector3 V = (v3 - v2) * 3;

	return v0 * (1 + 2 * t)*sSqr + v1 * (1 + 2 * s)*tSqr + U * sSqr*t - V * s*tSqr;
}

static inline Vector3 CatmullRomSplineLerp(const std::vector<Vector3> points, const float t)
{
	if (points.size() < 4)
		return Vector3();

	float s = 1 - t;
	float tSqr = t * t;
	float sSqr = s * s;

	std::vector<Vector3> pointVelocities(points.size(), Vector3());	// Create a vector of velocities, all zero to start

	for (int i = 1; i < points.size()-1; ++i)	// Loop through all points except beginning and end
	{
		pointVelocities[i] = (points[i + 1] - points[i - 1]) / 2.0f;
	}

	int splineT = static_cast<int>(floorf(t));
	float localT = t - static_cast<float>(splineT);

	Vector3 v0 = points[splineT];
	Vector3 v1 = points[splineT + 1];
	Vector3 U = pointVelocities[splineT];
	Vector3 V = pointVelocities[splineT + 1];

	return v0 * (1 + 2 * t)*sSqr + v1 * (1 + 2 * s)*tSqr + U * sSqr*t - V * s*tSqr;
}

}; // namespace Klink::JMath

#endif // INCLUDED_KLINK_JMATH_VECTOR3_H