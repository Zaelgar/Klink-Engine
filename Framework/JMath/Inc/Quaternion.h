#ifndef INCLUDED_KLINK_JMATH_QUATERNION_H
#define INCLUDED_KLINK_JMATH_QUATERNION_H

#include "Vector3.h"
#include "Matrix4.h"

namespace Klink::JMath
{

struct Quaternion
{
	union
	{
		struct { float x, y, z, w; };
		std::array<float, 4> q;
	};

	constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

	const static Quaternion Zero() { return { 0.0f, 0.0f, 0.0f, 0.0f }; }
	const static Quaternion Identity() { return { 0.0f, 0.0f, 0.0f, 1.0f }; }

	//static Quaternion RotationAxis(const Vector3& axis, float rad);
	static Quaternion RotationAxisDegrees(const Vector3& axis, float deg);
	static Quaternion EulerToQuaternion(const float xRad, const float yRad, const float zRad);
	static Quaternion EulerDegreesToQuaternion(const float xDeg, const float yDeg, const float zDeg);
	// Plus anything else ...

	constexpr Quaternion operator-()
	{
		return Quaternion(
			-x, -y, -z, -w
		);
	}
	constexpr Quaternion operator/ (const float& divisor) const
	{
		return Quaternion(x / divisor, y / divisor, z / divisor, w / divisor);
	}
	constexpr Quaternion operator+(const Quaternion& rhs) const
	{
		return Quaternion(

			(x+rhs.x),
			(y+rhs.y),
			(z+rhs.z),
			(w+rhs.w)

		);
	}
	constexpr Quaternion operator*(const Quaternion& rhs) const
	{
		return Quaternion(

			((w*rhs.x) + (x*rhs.w) + (y*rhs.z) - (z*rhs.y)),
			((w*rhs.y) - (x*rhs.z) + (y*rhs.w) + (z*rhs.x)),
			((w*rhs.z) + (x*rhs.y) - (y*rhs.x) + (z*rhs.w)),
			((w*rhs.w) - (x*rhs.x) - (y*rhs.y) - (z*rhs.z))

		);
	}
	constexpr Quaternion operator*(float s) const
	{
		return Quaternion(

			(s*x),
			(s*y),
			(s*z),
			(s*w)

		);
	}
	float Magnitude()
	{
		return sqrt((w*w) + (x*x) + (y*y) + (z*z));
	}
	Quaternion Normalize()
	{
		float check = ((w*w) + (x*x) + (y*y) + (z*z)) - 1;

		if (abs(check) > Epsilon)
			return *this / Magnitude();
		else
			return *this;
	}
	constexpr Quaternion Conjugate()
	{
		return Quaternion(-x, -y, -z, w);
	}
};

static Quaternion RotationAxis(const Vector3& axis, float rad)
{
	float angle = rad / 2;

	return Quaternion(

		(axis.x * sinf(angle)),
		(axis.y * sinf(angle)),
		(axis.z * sinf(angle)),
		(cosf(angle))

	);
}
static Quaternion RotationAxisDegrees(const Vector3& axis, float deg)
{
	float rad = deg * DegreesToRad;
	return RotationAxis(axis, rad);
}
static Quaternion EulerToQuaternion(const float xRad, const float yRad, const float zRad)
{
	// Abbreviations for the various angular functions // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	float cy = cos(xRad * 0.5f);
	float sy = sin(xRad * 0.5f);
	float cp = cos(yRad * 0.5f);
	float sp = sin(yRad * 0.5f);
	float cr = cos(zRad * 0.5f);
	float sr = sin(zRad * 0.5f);

	Quaternion q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = sy * cp * cr - cy * sp * sr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = cy * cp * sr - sy * sp * cr;

	return q;
}
static Quaternion EulerDegreesToQuaternion(const float xDeg, const float yDeg, const float zDeg)
{
	float xRad = xDeg * DegreesToRad;
	float yRad = yDeg * DegreesToRad;
	float zRad = zDeg * DegreesToRad;

	return EulerToQuaternion(xRad, yRad, zRad);
}
static Quaternion Slerp(Quaternion& q0, Quaternion& q1, float t)	// https://en.wikipedia.org/wiki/Slerp
{
	// Only unit quaternions are valid rotations.
	// Normalize to avoid undefined behavior.
	q0.Normalize();
	q1.Normalize();

	// Compute the cosine of the angle between the two vectors.
	float dot = Dot(Vector3(q0.x, q0.y, q0.z), Vector3(q1.x, q1.y, q1.z));

	// If the dot product is negative, slerp won't take
	// the shorter path. Note that v1 and -v1 are equivalent when
	// the negation is applied to all four components. Fix by 
	// reversing one quaternion.
	if (dot < 0.0f) {
		q1 = -q1;
		dot = -dot;
	}

	//const float DOT_THRESHOLD = 0.9995;
	//if (dot > DOT_THRESHOLD) {
	//	// If the inputs are too close for comfort, linearly interpolate
	//	// and normalize the result.

	//	Quaternion result = q0 + t * (q1 - q0);
	//	result.Normalize();
	//	return result;
	//}


	// Since dot is in range [0, DOT_THRESHOLD], acos is safe
	float theta_0 = acos(dot);        // theta_0 = angle between input vectors
	float theta = theta_0 * t;          // theta = angle between v0 and result
	float sin_theta = sin(theta);     // compute this value only once
	float sin_theta_0 = sin(theta_0); // compute this value only once

	float s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
	float s1 = sin_theta / sin_theta_0;

	return (q0 * s0) + (q1 * s1);
}

} // namespace Klink::JMath

#endif // #ifndef INCLUDED_KLINK_JMATH_QUATERNION_H