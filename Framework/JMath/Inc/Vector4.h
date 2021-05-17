#ifndef INCLUDED_KLINK_JMATH_VECTOR4_H
#define INCLUDED_KLINK_JMATH_VECTOR4_H

#include "Common.h"
#include "Constants.h"

using namespace Klink::JMath::Constants;

namespace Klink::JMath
{

struct Vector4
{
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
	union { float w; float a; };

	// Constructors
	constexpr Vector4()										noexcept : Vector4(0.0f) {}
	constexpr Vector4(const Vector4& other)					noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}
	constexpr Vector4(float f)								noexcept : x(f), y(f), z(f), w(1.0f) {}
	constexpr Vector4(float x, float y, float z)			noexcept : x(x), y(y), z(z), w(1.0f) {}
	constexpr Vector4(float x, float y, float z, float w)	noexcept : x(x), y(y), z(z), w(w) {}

	// Overloaded Operators
	Vector4 operator+(const Vector4& other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vector4 operator-(const Vector4& other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	Vector4 operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}
	Vector4 operator*(const int& power) const
	{
		return Vector4(x * power, y * power, z * power, w * power);
	}
	Vector4 operator*(const float& power) const
	{
		return Vector4(x * power, y * power, z * power, w * power);
	}
	Vector4 operator/(const int& power) const
	{
		return Vector4(x / power, y / power, z / power, w / power);
	}
	Vector4 operator/(const float& power) const
	{
		return Vector4(x / power, y / power, z / power, w / power);
	}
	Vector4 &operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	Vector4 &operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	Vector4 &operator*=(const int& power)
	{
		x *= power;
		y *= power;
		z *= power;
		w *= power;
		return *this;
	}
	Vector4 &operator*=(const float& power)
	{
		x *= power;
		y *= power;
		z *= power;
		w *= power;
		return *this;
	}
	Vector4 &operator/=(int power)
	{
		x /= power;
		y /= power;
		z /= power;
		w /= power;
		return *this;
	}
	Vector4 &operator/=(float power)
	{
		x /= power;
		y /= power;
		z /= power;
		w /= power;
		return *this;
	}
	Vector4 &operator=(const Vector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
	bool operator==(const Vector4& other) const
	{
		return (fabs(x - other.x) < Epsilon && fabs(y - other.y) < Epsilon && fabs(z - other.z) < Epsilon && fabs(w - other.w) < Epsilon);
	}
	bool operator!=(const Vector4& other) const
	{
		return (x > other.x || x < other.x || y > other.y || y < other.y || z > other.z || z < other.z || w > other.w || w < other.w);
	}
	// Functions and Methods
	inline float Length() const
	{
		return (float)sqrt(x*x + y * y + z * z + w * w);
	}
	inline float LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}
	inline void Normalize()
	{
		*this /= Length();
	}
	inline Vector4 Normalized() const
	{
		return *this / Length();
	}
};

	static inline float Dot(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static inline float Distance(Vector4& v1, Vector4& v2)
	{
		return (sqrt(

			((v1.x - v2.x)*(v1.x - v2.x)) +
			((v1.y - v2.y)*(v1.y - v2.y)) +
			((v1.z - v2.z)*(v1.z - v2.z)) +
			((v1.w - v2.w)*(v1.w - v2.w))

		));
	}

}; // namespace Klink::JMath
#endif // INCLUDED_KLINK_JMATH_VECTOR4_H