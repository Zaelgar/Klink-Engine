#ifndef INCLUDED_KLINK_JMATH_MATRIX3_H
#define INCLUDED_KLINK_JMATH_MATRIX3_H

#include "Vector2.h"

namespace Klink::JMath
{

struct Matrix3
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3()
		: _11(1.0f), _12(0.0f), _13(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f)
	{}

	Matrix3(
		float _11, float _12, float _13,
		float _21, float _22, float _23,
		float _31, float _32, float _33)
		: _11(_11), _12(_12), _13(_13)
		, _21(_21), _22(_22), _23(_23)
		, _31(_31), _32(_32), _33(_33)
	{}

	// Helper functions
	static Matrix3 Zero() { return Matrix3(); }
	static Matrix3 Identity() { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Translation(float x, float y) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, 1.0f); }
	static Matrix3 Translation(const Vector2& v) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, 1.0f); }
	static Matrix3 Rotation(float rad) { return Matrix3(cosf(rad), sinf(rad), 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(float s) { return Matrix3(s, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(float sx, float sy) { return Matrix3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(const Vector2& s) { return Matrix3(s.x, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 1.0f); }

	static inline Matrix3 Transpose(const Matrix3& m)
	{
		return Matrix3
		(
			m._11, m._21, m._31,
			m._12, m._22, m._32,
			m._13, m._23, m._33
		);
	}
	static inline float Determinant(const Matrix3& m)
	{
		float det = 0.0f;
		det = (m._11 * (m._22 * m._33 - m._23 * m._32));
		det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
		det += (m._13 * (m._21 * m._32 - m._22 * m._31));
		return det;
	}
	static inline Matrix3 Adjoint(const Matrix3& m)
	{
		return Matrix3
		(
			(m._22 * m._33 - m._23 * m._32),
			-(m._12 * m._33 - m._13 * m._32),
			(m._12 * m._23 - m._13 * m._22),

			-(m._21 * m._33 - m._23 * m._31),
			(m._11 * m._33 - m._13 * m._31),
			-(m._11 * m._23 - m._13 * m._21),

			(m._21 * m._32 - m._22 * m._31),
			-(m._11 * m._32 - m._12 * m._31),
			(m._11 * m._22 - m._12 * m._21)
		);
	}
	static inline Matrix3 Inverse(const Matrix3& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}

	// Self operator overloads
	Matrix3 operator-() const
	{
		return Matrix3(
			-_11, -_12, -_13,
			-_21, -_22, -_23,
			-_31, -_32, -_33);
	}
	Matrix3 operator+(const Matrix3& rhs) const
	{
		return Matrix3(
			_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
			_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
			_31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
	}
	Matrix3 operator-(const Matrix3& rhs) const
	{
		return Matrix3(
			_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
			_21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
			_31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
	}
	Matrix3 operator*(const Matrix3& rhs) const
	{
		return Matrix3(
			(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31),
			(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32),
			(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33),

			(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31),
			(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32),
			(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33),

			(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31),
			(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32),
			(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33));
	}
	Matrix3 operator*(float s) const
	{
		return Matrix3(
			_11 * s, _12 * s, _13 * s,
			_21 * s, _22 * s, _23 * s,
			_31 * s, _32 * s, _33 * s);
	}
	Matrix3 operator/(float s) const
	{
		return Matrix3(
			_11 / s, _12 / s, _13 / s,
			_21 / s, _22 / s, _23 / s,
			_31 / s, _32 / s, _33 / s);
	}
	Matrix3 operator+=(const Matrix3& rhs)
	{
		_11 += rhs._11; _12 += rhs._12; _13 += rhs._13;
		_21 += rhs._21; _22 += rhs._22; _23 += rhs._23;
		_31 += rhs._31; _32 += rhs._32; _33 += rhs._33;
		return *this;
	}

	// Other math class overloads
	friend Vector3& operator*=(Vector3& v, const Matrix3& matrix)
	{
		v.x = ((matrix._11 * v.x) + (matrix._12 * v.y) + (matrix._13 * v.z));
		v.y = ((matrix._21 * v.x) + (matrix._22 * v.y) + (matrix._23 * v.z));
		v.z = ((matrix._31 * v.x) + (matrix._32 * v.y) + (matrix._33 * v.z));

		return v;
	}
	friend Vector2 operator*(Vector2& v, const Matrix3& matrix)
	{
		return Vector2
		(
			v.x * matrix._11 + v.y * matrix._21 + matrix._31,
			v.x * matrix._12 + v.y * matrix._22 + matrix._32
		);
	}
	static inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
	{
		return Vector2
		(
			v.x * m._11 + v.y * m._21 + m._31,
			v.x * m._12 + v.y * m._22 + m._32
		);
	}
	static inline Vector2 TransformNormal(const Vector2& v, const Matrix3& m)
	{
		return Vector2
		(
			v.x * m._11 + v.y * m._21,
			v.x * m._12 + v.y * m._22
		);
	}
};

} // namespace Klink::JMath

#endif // INCLUDED_KLINK_JMATH_MATRIX3_H