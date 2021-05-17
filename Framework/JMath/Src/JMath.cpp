#include "Precompiled.h"
#include "JMath.h"

namespace Klink::JMath
{

bool PointInCircle(const Vector2& point, const Circle& circle)
{
	const Vector2 centerToPoint = point - circle.center;
	const float distSqr = Vector2::Dot(centerToPoint, centerToPoint);
	const float radiusSqr = circle.radius * circle.radius;
	return distSqr < radiusSqr;
}

bool Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = startToEnd.Magnitude();
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Vector2::Dot(startToCenter, dir);
	Vector2 point;
	if (projection > len)
	{
		point = l.to;
	}
	else if (projection < 0.0f)
	{
		point = l.from;
	}
	else
	{
		point = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	if (!PointInCircle(point, c))
		return false;

	if (closestPoint)
		*closestPoint = point;

	return true;
}

bool Intersect(const LineSegment& a, const LineSegment& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

bool Intersect(Klink::JMath::Vector2 A, Klink::JMath::Vector2 B, Klink::JMath::Vector2 C, Klink::JMath::Vector2 D, float & dist, Klink::JMath::Vector2 & point)
{
	float rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
	float rBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

	float sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);
	float sBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}

	float r = rTop / rBot;
	float s = sTop / sBot;

	if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
	{
		dist = Vector2::Distance(A, B) * r;

		Vector2 t = (B - A);
		t = t * r;

		point = A + t;

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}

bool Intersect(LineSegment a, LineSegment b, float& dist, Vector2& point)
{
	return Intersect(a.from, a.to, b.from, b.to, dist, point);
}

Matrix4 RotationQuaternionToMatrix(const Quaternion& q)
{
	return Matrix4(

		(1.0f - (2.0f*q.y*q.y) - (2.0f*q.z*q.z)), (2.0f*q.x*q.y) - (2.0f*q.w*q.z), (2.0f*q.x*q.z) + (2.0f*q.w*q.y), (0),
		((2.0f*q.x*q.y) + (2.0f*q.w*q.z)), (1.0f - (2.0f*q.x*q.x) - (2.0f*q.z*q.z)), ((2.0f*q.y*q.z) - (2.0f*q.w*q.x)), (0),
		((2.0f*q.x*q.z) - (2.0f*q.w*q.y)), ((2.0f*q.y*q.z) + (2.0f*q.w*q.x)), (1.0f - (2.0f*q.x*q.x) - (2.0f*q.y*q.y)), (0),
		(0.0f), (0.0f), (0.0f), (1.0f)

	);
}

Matrix4 CreateTransformMatrix(const Vector3 & position, const Quaternion & rotation, const Vector3 & scale)
{
	return (Matrix4::Scaling(scale) * RotationQuaternionToMatrix(rotation) * Matrix4::Translation(position));
}

void RandomPointsInRectangle(std::vector<Klink::JMath::Vector2>& points, Klink::JMath::Rect areaBounds, int numberOfPoints)
{
	if (numberOfPoints < 1)
		return;

	// Modulo operations require int or enum type. Floor maximum to integer closest inside the bounds. Ceiling minimum to closest inside bounds.
	int maxX, maxY, minX, minY;
	maxX = static_cast<int>(floor(areaBounds.max.x));
	maxY = static_cast<int>(floor(areaBounds.max.y));
	minX = static_cast<int>(ceil(areaBounds.min.x));
	minY = static_cast<int>(ceil(areaBounds.min.y));

	float x, y;
	for (int index = 0; index < numberOfPoints; ++index)
	{
		x = (rand() % (maxX - minX)) + minX;
		y = (rand() % (maxY - minY)) + minY;

		points.emplace_back(Vector2(x, y));
	}
}


/// Splines

} // namespace Klink::JMath