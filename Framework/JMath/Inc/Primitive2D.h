#ifndef INCLUDED_JMATH_PRIMITIVE2D_H
#define INCLUDED_JMATH_PRIMITIVE2D_H

//-------------------------------------------------------------------------------------------------
// Primitives taken from X.sln by Peter Chan
//-------------------------------------------------------------------------------------------------

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3.h"



namespace Klink::JMath
{

struct Circle
{
	Vector2 center;
	float radius;

	Circle() : center(0.0f, 0.0f), radius(0.0f) {}
	Circle(const Vector2& center, float radius) : center(center), radius(radius) {}
	Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
};

struct Rect
{
	union
	{
		struct { float left, top; };
		Vector2 min;
	};
	union
	{
		struct { float right, bottom; };
		Vector2 max;
	};

	Rect() : min{ 0.0f,0.0f }, max{ 1.0f, 1.0f } {}
	Rect(float left, float top, float right, float bottom) : left{ left }, top{ top }, right{ right }, bottom{ bottom } {}
	~Rect()
	{}
};

struct Rectangle
{
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomRight;
	Vector2 bottomLeft;

	Rectangle() : topLeft{ 0.0f, 0.0f }, topRight{ 10.0f, 0.0f }, bottomRight{ 10.0f, 10.0f }, bottomLeft{ 0.0f, 10.0f }{}
	Rectangle(Vector2 TopLeft, Vector2 TopRight, Vector2 BottomRight, Vector2 BottomLeft)
		: topLeft(TopLeft),
		topRight(TopRight),
		bottomRight(BottomRight),
		bottomLeft(BottomLeft)
	{}
};

struct LineSegment
{
	Vector2 from;
	Vector2 to;

	LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
	LineSegment(float x0, float y0, float x1, float y1) : from(x0, y0), to(x1, y1) {}
	LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}
};

struct Triangle
{
	Vector2 pointOne;
	Vector2 pointTwo;
	Vector2 pointThree;
	Vector2 center;

	Triangle() : pointOne{ 10.0f, 0.0f }, pointTwo{ 0.0f, 5.0f }, pointThree{ 0.0f, -5.0f }, center{ 0.0f, 0.0f }{}
	Triangle(Vector2 one, Vector2 two, Vector2 three) : pointOne{ one }, pointTwo{ two }, pointThree{ three }{center = Vector2{ (one.x + two.x + three.x) / 3.0f, (one.y + two.y + three.y) / 3.0f }; };
};

struct AABB
{
	Vector2 center;
	Vector2 extend;
};

struct OBB
{
	Vector2 center;
	Vector2 extend;
};

}; //  namespace

#endif // INCLUDED_JMATH_PRIMITIVE2D_H