// Filename:	math.h
// Created by:	Jaidon van Herwaarden
// ----------------------------------------

#ifndef INCLUDED_KLINK_JMATH_H
#define INCLUDED_KLINK_JMATH_H

#include "Common.h"
#include "Constants.h"
#include "MetaRegistration.h"

// Shapes
#include "Primitive2D.h"

// Vectors
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

// Matrices
#include "Matrix4.h"
#include "Matrix3.h"

// Quaternion
#include "Quaternion.h"

// Random
#include "Random.h"

namespace Klink::JMath
{
	bool PointInCircle(const Klink::JMath::Vector2& point, const Klink::JMath::Circle& circle);
	bool Intersect(const Klink::JMath::Circle& c, const Klink::JMath::LineSegment& l, Klink::JMath::Vector2* closestPoint);
	bool Intersect(const LineSegment& a, const LineSegment& b);
	bool Intersect(Klink::JMath::Vector2 A, Klink::JMath::Vector2 B, Klink::JMath::Vector2 C, Klink::JMath::Vector2 D, float& dist, Klink::JMath::Vector2& point);
	bool Intersect(LineSegment a, LineSegment b, float& dist, Vector2& point);

	Matrix4 RotationQuaternionToMatrix(const Quaternion& q);

	Matrix4 CreateTransformMatrix(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

	void RandomPointsInRectangle(std::vector<Klink::JMath::Vector2>& points, Klink::JMath::Rect areaBounds, int numberOfPoints);
};

#endif // #ifndef INCLUDED_KLINK_JMATH_H