#include "Precompiled.h"
#include "Object.h"

using namespace Klink::Physics;
using namespace Klink::JMath;
using namespace Klink::Graphics;


void Object2D::Initialize(int _pointCount, float _radius, Klink::JMath::Vector2 _position, Klink::JMath::Vector2 _velocity, Colours::Colour _colour)
{
	// Initialize Particle
	radius = _radius;
	position = _position;
	velocity = _velocity;
	colour = _colour;

	// Build polygon based on points (at least 3 for a triangle)
	if (_pointCount < 3)
		_pointCount = 3;
	mPolygonPoints.reserve(_pointCount);
	Vector2 point;
	for (int pointIndex = 0; pointIndex < _pointCount; ++pointIndex)	// Each polygon point is built randomly on a unit circle, then extended by the radius
	{
		point = RandomOnUnitCircle() * radius;
		mPolygonPoints.emplace_back(point);
	}
	ReorderPointsForConvexHull();										// Since points can be anywhere on the unit circle, we reorder for easy drawing (note: not completely convex when two points are too close together)

	// Initialize AABB
	float maxX = -1000000.f;	// HACK
	float minX = 1000000.f;		// HACK
	float maxY = -1000000.f;	// HACK
	float minY = 1000000.f;		// HACK
	float avgX = 0.f;
	float avgY = 0.f;
	for (auto point : mPolygonPoints)
	{
		avgX += point.x;
		avgY += point.y;
		if (point.x > maxX)
			maxX = point.x;
		if (point.x < minX)
			minX = point.x;
		if (point.y > maxY)
			maxY = point.y;
		if (point.y < minY)
			minY = point.y;
	}
	mPolygonCenter.x = avgX / mPolygonPoints.size();
	mPolygonCenter.y = avgY / mPolygonPoints.size();

	// Find largest distance between points
	float largestDistance = 0.0f;
	Vector2 p1;
	Vector2 p2;
	for (auto point : mPolygonPoints)
	{
		for (auto otherPoint : mPolygonPoints)
		{
			if (point == otherPoint)	// Skip point comparison for same points
				continue;

			float distance = Vector2::DistanceSquared(point, otherPoint);
			if (distance > largestDistance)
			{
				p1 = point;
				p2 = otherPoint;
				largestDistance = distance;
			}
		}
	}
	mCircleBoundingBox.radius = sqrt(largestDistance) / 2.0f;
	mCircleBoundingBox.center = (p1 + p2) / 2.0f;

	mAABB.center.x = ((maxX + minX) / 2.0f);
	mAABB.center.y = ((maxY + minY) / 2.0f);
	mAABB.extend.x = (maxX - minX) / 2.0f;
	mAABB.extend.y = (maxY - minY) / 2.0f;
}


void Object2D::Update(float deltaTime)
{
}


void Object2D::Draw()
{
	Vector2 finalPosition1 = Vector2::Zero();
	Vector2 finalPosition2 = Vector2::Zero();

	int size = mPolygonPoints.size();
	for (int pointIndex = 0; pointIndex < size; ++pointIndex)
	{
		Vector2* p1 = &mPolygonPoints[pointIndex];
		Vector2* p2;
		if (pointIndex == size - 1)		// If last point in list, draw line to first point
			p2 = &mPolygonPoints[0];
		else
			p2 = &mPolygonPoints[pointIndex + 1];

		finalPosition1 = *p1 + position;
		finalPosition2 = *p2 + position;

		SimpleDraw::DrawScreenLine(finalPosition1, finalPosition2, colour);
	}
	SimpleDraw::DrawScreenCircle(mPolygonCenter + position, 5.0f, colour);
}


void Object2D::DrawAABB()
{
	Vector2 c = position + mAABB.center;

	Vector2 topLeft(c - mAABB.extend);
	Vector2 bottomRight(c + mAABB.extend);
	SimpleDraw::DrawScreenRect(topLeft, bottomRight, Colours::Red);
	SimpleDraw::DrawScreenCircle(mAABB.center + position, 5.0f, Colours::Red);
}


void Object2D::DrawCircleBoundingBox()
{
	SimpleDraw::DrawScreenCircle(mCircleBoundingBox.center + position, mCircleBoundingBox.radius);	// Draw Bounds
	SimpleDraw::DrawScreenCircle(mCircleBoundingBox.center + position, 5.0f);						// Draw Center
}


void Object2D::DrawVelocity()
{
	SimpleDraw::DrawScreenLine(position, position + velocity, Colours::Green);
}


void Object2D::ReorderPointsForConvexHull()
{
	for (int testIndex = 0; testIndex < mPolygonPoints.size(); ++testIndex)
	{
		int minDistIndex = -1;
		float minDist = 1000000000.0f;	// HACK
		for (int otherIndex = testIndex; otherIndex < mPolygonPoints.size(); ++otherIndex)
		{
			if (testIndex == otherIndex)
				continue;

			float distance = Vector2::Distance(mPolygonPoints[testIndex], mPolygonPoints[otherIndex]);
			if (distance < minDist)
			{
				minDist = distance;
				minDistIndex = otherIndex;
			}
		}
		// Once closest point to testIndex is found, make sure it is the next point in the chain
		if (testIndex + 1 != minDistIndex && minDistIndex != -1)	// if the closest point is not already the next point
			std::swap(mPolygonPoints[testIndex + 1], mPolygonPoints[minDistIndex]);
	}
}


void Object2D::SetPosition(const Vector2& pos)
{
	position = pos;
	lastPosition = pos;
}


void Object2D::SetVelocity(const Vector2& vel)
{
	velocity = vel;
	lastPosition = position - vel;
}

/*

// Slant wall collision with OBB
// AABB collision
// Circle collision with bounding box

*/