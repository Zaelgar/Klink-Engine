#ifndef INCLUDED_KLINK_JMATH_MATRIX4_H
#define INCLUDED_KLINK_JMATH_MATRIX4_H

#include "Common.h"
#include "Constants.h"

#include "Vector3.h"
#include "Quaternion.h"

using namespace Klink::JMath::Constants;

namespace Klink::JMath
{


struct Matrix4
{
	union
		{
			struct // row+column subscript
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};

	Matrix4() noexcept : Matrix4(0.0f) {}
	Matrix4(float f) noexcept
			: _11(f), _12(f), _13(f), _14(f),
			_21(f), _22(f), _23(f), _24(f),
			_31(f), _32(f), _33(f), _34(f),
			_41(f), _42(f), _43(f), _44(f)
		{}
	Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
			: _11(_11), _12(_12), _13(_13), _14(_14),
			_21(_21), _22(_22), _23(_23), _24(_24),
			_31(_31), _32(_32), _33(_33), _34(_34),
			_41(_41), _42(_42), _43(_43), _44(_44)
		{}

	static Matrix4 Zero() { return Matrix4(); }
	static Matrix4 Identity() { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Translation(float x, float y, float z) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }
	static Matrix4 Translation(const Vector3& v) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f); }
	static Matrix4 RotationX(float rad) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 RotationY(float rad) { return Matrix4(cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 RotationZ(float rad) { return Matrix4(cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(float s) { return Matrix4(s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(float sx, float sy, float sz) { return Matrix4(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(const Vector3& s) { return Matrix4(s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static inline Matrix4 Rotation(Vector3 v) { return Matrix4(RotationX(v.x) * RotationY(v.y) * RotationZ(v.z)); }

	Matrix4 operator-() const
		{
			return Matrix4(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44);
		}
	Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
		}
	Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
		}
	Matrix4 operator*(const Matrix4& rhs) const
		{
			return Matrix4(
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
				(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
				(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
				(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

				(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
				(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
				(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
				(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44));
		}
	Matrix4 operator*(float s) const
		{
			return Matrix4(
				_11 * s, _12 * s, _13 * s, _14 * s,
				_21 * s, _22 * s, _23 * s, _24 * s,
				_31 * s, _32 * s, _33 * s, _34 * s,
				_41 * s, _42 * s, _43 * s, _44 * s);
		}
	Matrix4 operator/(float s) const
		{
			return Matrix4(
				_11 / s, _12 / s, _13 / s, _14 / s,
				_21 / s, _22 / s, _23 / s, _24 / s,
				_31 / s, _32 / s, _33 / s, _34 / s,
				_41 / s, _42 / s, _43 / s, _44 / s);
		}
	Matrix4 operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}
	bool operator==(const Matrix4& other) const
		{
			return(

				fabs(_11 - other._11) < Epsilon && fabs(_12 - other._12) < Epsilon && fabs(_13 - other._13) < Epsilon && fabs(_14 - other._14) < Epsilon &&
				fabs(_21 - other._21) < Epsilon && fabs(_22 - other._22) < Epsilon && fabs(_23 - other._23) < Epsilon && fabs(_24 - other._24) < Epsilon &&
				fabs(_31 - other._31) < Epsilon && fabs(_32 - other._32) < Epsilon && fabs(_33 - other._33) < Epsilon && fabs(_34 - other._34) < Epsilon &&
				fabs(_41 - other._41) < Epsilon && fabs(_42 - other._42) < Epsilon && fabs(_43 - other._43) < Epsilon && fabs(_44 - other._44) < Epsilon

				);
		}
	bool operator!=(const Matrix4& other) const
		{
			return(

				fabs(_11 - other._11) > Epsilon || fabs(_12 - other._12) > Epsilon || fabs(_13 - other._13) > Epsilon || fabs(_14 - other._14) > Epsilon ||
				fabs(_21 - other._21) > Epsilon || fabs(_22 - other._22) > Epsilon || fabs(_23 - other._23) > Epsilon || fabs(_24 - other._24) > Epsilon ||
				fabs(_31 - other._31) > Epsilon || fabs(_32 - other._32) > Epsilon || fabs(_33 - other._33) > Epsilon || fabs(_34 - other._34) > Epsilon ||
				fabs(_41 - other._41) > Epsilon || fabs(_42 - other._42) > Epsilon || fabs(_43 - other._43) > Epsilon || fabs(_44 - other._44) > Epsilon

				);
		}


	// Returns the transpose of this* matrix
	Matrix4 Transpose() const
		{
			return Matrix4(
				(_11), (_21), (_31), (_41),
				(_12), (_22), (_32), (_42),
				(_13), (_23), (_33), (_43),
				(_14), (_24), (_34), (_44)
			);
		}
	static inline Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4
		(
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		);
	}
	// Returns the determinate of this* matrix
	float Determinate() const
		{
			return (

				_11*(_22*_33*_44 + _23 * _34*_42 + _24 * _32*_43 - _24 * _33*_42 - _23 * _32*_44 - _22 * _34*_43)

				- _21 * (_12*_33*_44 + _13 * _34*_42 + _14 * _32*_43 - _14 * _33*_42 - _13 * _32*_44 - _12 * _34*_43)

				+ _31 * (_12*_23*_44 + _13 * _24*_42 + _14 * _22*_43 - _14 * _23*_42 - _13 * _22*_44 - _12 * _24*_43)

				- _41 * (_12*_23*_34 + _13 * _24*_32 + _14 * _22*_33 - _14 * _23*_32 - _13 * _22*_34 - _12 * _24*_33)

				);
		}
	// Returns the adjugate of this* matrix
	Matrix4 Adjugate() const
		{
			// https://semath.info/src/inverse-cofactor-ex4.html
			return Matrix4(

				(_22*_33*_44 + _23 * _34*_42 + _24 * _32*_43 - _24 * _33*_42 - _23 * _32*_44 - _22 * _34*_43), (-_12 * _33*_44 - _13 * _34*_42 - _14 * _32*_43 + _14 * _33*_42 + _13 * _32*_44 + _12 * _34*_43), (_12*_23*_44 + _13 * _24*_42 + _14 * _22*_43 - _14 * _23*_42 - _13 * _22*_44 - _12 * _24*_43), (-_12 * _23*_34 - _13 * _24*_32 - _14 * _22*_33 + _14 * _23*_32 + _13 * _22*_34 + _12 * _24*_33),

				(-_21 * _33*_44 - _23 * _34*_41 - _24 * _31*_43 + _24 * _33*_41 + _23 * _31*_44 + _21 * _34*_43), (_11*_33*_44 + _13 * _34*_41 + _14 * _31*_43 - _14 * _33*_41 - _13 * _31*_44 - _11 * _34*_43), (-_11 * _23*_44 - _13 * _24*_41 - _14 * _21*_43 + _14 * _23*_41 + _13 * _21*_44 + _11 * _24*_43), (_11*_23*_34 + _13 * _24*_31 + _14 * _21*_33 - _14 * _23*_31 - _13 * _21*_34 - _11 * _24*_33),

				(_21*_32*_44 + _22 * _34*_41 + _24 * _31*_42 - _24 * _32*_41 - _22 * _31*_44 - _21 * _34*_42), (-_11 * _32*_44 - _12 * _34*_41 - _14 * _31*_42 + _14 * _32*_41 + _12 * _31*_44 + _11 * _34*_42), (_11*_22*_44 + _12 * _24*_41 + _14 * _21*_42 - _14 * _22*_41 - _12 * _21*_44 - _11 * _24*_42), (-_11 * _22*_34 - _12 * _24*_31 - _14 * _21*_32 + _14 * _22*_31 + _12 * _21*_34 + _11 * _24*_32),

				(-_21 * _32*_43 - _22 * _33*_41 - _23 * _31*_42 + _23 * _32*_41 + _22 * _31*_43 + _21 * _33*_42), (_11*_32*_43 + _12 * _33*_41 + _13 * _31*_42 - _13 * _32*_41 - _12 * _31*_43 - _11 * _33*_42), (-_11 * _22*_43 - _12 * _23*_41 - _13 * _21*_42 + _13 * _22*_41 + _12 * _21*_43 + _11 * _23*_42), (_11*_22*_33 + _12 * _23*_31 + _13 * _21*_32 - _13 * _22*_31 - _12 * _21*_33 - _11 * _23*_32)

			);
		}
	// Returns the inverse of this* matrix
	Matrix4 Inverse() const
		{
			float factor = 1 / Determinate();

			Matrix4 inverse = Adjugate();

			inverse = inverse * factor;

			return  inverse;
		}

	friend Vector3& operator*=(Vector3& v, const Matrix4& matrix)
		{
			v.x = ((matrix._11 * v.x) + (matrix._12 * v.y) + (matrix._13 * v.z));
			v.y = ((matrix._21 * v.x) + (matrix._22 * v.y) + (matrix._23 * v.z));
			v.z = ((matrix._31 * v.x) + (matrix._32 * v.y) + (matrix._33 * v.z));

			return v;
		}
	static inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		return Vector3
		(
			v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
			v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
			v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
		);
	}
	static inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return Vector3
		(
			v.x * m._11 + v.y * m._21 + v.z * m._31,
			v.x * m._12 + v.y * m._22 + v.z * m._32,
			v.x * m._13 + v.y * m._23 + v.z * m._33
		);
	}

	static Matrix4 MatrixRotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return Matrix4
		(
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}

};




}; // namespace Klink::JMath

#endif // INCLUDED_KLINK_JMATH_MATRIX4_H