#include "Precompiled.h"

#include "Object.h"
#include "PhysicsWorld.h"

using namespace Klink;
using namespace Klink::Physics;
using namespace Klink::Graphics;

void PhysicsWorld::Initialize(const PhysicsSettings& settings)
{
	mSettings = settings;
	mObject3s.reserve(mSettings.maxParticles);
	mObject2s.reserve(mSettings.maxParticles);
}

void PhysicsWorld::Terminate()
{
	ClearDynamic();
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces();
		Integrate();
		SatisfyConstraints(deltaTime);
	}
}

void PhysicsWorld::DebugDraw() const
{
}

void PhysicsWorld::AddObject(Object3D* p)
{
	mObject3s.push_back(p);
}

void PhysicsWorld::AddObject(Object2D* p)
{
	mObject2s.push_back(p);
}

void PhysicsWorld::ChangeCollisionType(Klink::Physics::CollisionType collisionType)
{
	mSettings.mCollisionType = collisionType;
}

void PhysicsWorld::ClearDynamic()
{
	for (auto p : mObject3s)
	{
		delete p;
	}
	mObject3s.clear();

	//for (int index = 0; index < mObject2s.size(); ++index)
	//{
	//	delete mObject2s[index];
	//}
	mObject2s.clear();
}

void PhysicsWorld::AccumulateForces()
{
	for (auto p : mObject3s)
	{
		p->acceleration = mSettings.gravity;
	}
	for (auto p : mObject2s)
	{
		p->acceleration.y = mSettings.gravity.y;
	}
}

void PhysicsWorld::Integrate()
{
	const float gamma = 1.0f - mSettings.drag;
	const float timeStepSqr = mSettings.timeStep * mSettings.timeStep;
	for (auto p : mObject3s)
	{
		JMath::Vector3 displacement = (p->position - p->lastPosition)* gamma + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = p->position + displacement;
	}
	for (auto p : mObject2s)
	{
		p->velocity += p->acceleration * mSettings.timeStep;
		p->position += p->velocity * mSettings.timeStep;
	}
}

void PhysicsWorld::SatisfyConstraints(float deltaTime)
{
	switch (mSettings.mCollisionType)
	{
	case CollisionType::CircleBoundingBox:
		ResolveCircleBoundCollision(deltaTime);
		break;

	case CollisionType::AABB:
		ResolveAABBCollision(deltaTime);
		break;
	}
}

void PhysicsWorld::ResolveCircleBoundCollision(float deltaTime)
{
	if (mSettings.IsScreenBoundary)
	{
		int xSize = GraphicsSystem::Get()->GetBackBufferWidth();
		int ySize = GraphicsSystem::Get()->GetBackBufferHeight();

		Vector2 pos;

		for (auto p : mObject2s)
		{
			pos = p->position + p->mCircleBoundingBox.center;

			if (pos.x < p->mCircleBoundingBox.radius)
			{
				p->position.x = p->mCircleBoundingBox.radius - p->mCircleBoundingBox.center.x;
				p->velocity.x *= -1;
			}
			else if (pos.x > xSize - p->mCircleBoundingBox.radius)
			{
				p->position.x = xSize - p->mCircleBoundingBox.radius - p->mCircleBoundingBox.center.x;
				p->velocity.x *= -1;
			}
			if (pos.y < p->mCircleBoundingBox.radius)
			{
				p->position.y = p->mCircleBoundingBox.radius - p->mCircleBoundingBox.center.y;
				p->velocity.y *= -1;
			}
			else if (pos.y > ySize - p->mCircleBoundingBox.radius)
			{
				p->position.y = ySize - p->mCircleBoundingBox.radius - p->mCircleBoundingBox.center.y;
				p->velocity.y *= -1;
			}
		}
	}

	Object2D* object = nullptr;
	float objectRadius = 0.f;
	Vector2 objectCenter = Vector2::Zero();

	Object2D* otherObject = nullptr;
	float otherRadius = 0.f;
	Vector2 otherCenter = Vector2::Zero();

	for (size_t objectIndex = 0; objectIndex < mObject2s.size()-1; ++objectIndex)
	{
		object = mObject2s[objectIndex];
		objectRadius = object->mCircleBoundingBox.radius;
		objectCenter = object->position + object->mCircleBoundingBox.center;	// Circle Bounding box "center" is an offset from the actual position because centers are calculated differently for different collision types

		for (size_t otherIndex = objectIndex+1; otherIndex < mObject2s.size(); ++otherIndex)
		{
			otherObject = mObject2s[otherIndex];
			otherRadius = otherObject->mCircleBoundingBox.radius;
			otherCenter = otherObject->position + otherObject->mCircleBoundingBox.center;

			float distanceSqr = Vector2::DistanceSquared(objectCenter, otherCenter);
			float distance = sqrt(distanceSqr);

			// Compare distance of centers with sum of radii. If distance is less than sum, we have intersection
			if ((objectRadius + otherRadius) > distance)
			{
				float t = (objectRadius*objectRadius - otherRadius*otherRadius + distanceSqr) / (distance*2.0f);
				Vector2 contactPoint = (((otherCenter - objectCenter) / distance) * t) + objectCenter;

				// Calculate minimum distance separation and separate
				float minimumDistance = (objectRadius + otherRadius + 1.0f)/2.0f;
				object->position -= object->velocity.Normalized() * minimumDistance * deltaTime;
				otherObject->position -= otherObject->velocity.Normalized() * minimumDistance * deltaTime;

				// Reflect Velocities
				Vector2 contactNormal = (otherCenter - objectCenter).Normalized();

				Vector2 objectVelocityPerpendicular = contactNormal * Vector2::Dot(object->velocity, contactNormal);
				Vector2 otherVelocityPerpendicular = contactNormal * Vector2::Dot(otherObject->velocity, contactNormal);

				Vector2 objectVelocityNew = object->velocity - objectVelocityPerpendicular;
				Vector2 otherVelocityNew = otherObject->velocity - otherVelocityPerpendicular;

				Vector2 objectVelocityPrime = objectVelocityNew - objectVelocityPerpendicular;
				Vector2 otherVelocityPrime = otherVelocityNew - otherVelocityPerpendicular;

				object->velocity = objectVelocityPrime;
				otherObject->velocity = otherVelocityPrime;
			}
		}
	}
}

void PhysicsWorld::ResolveAABBCollision(float deltaTime)
{
	if (mSettings.IsScreenBoundary)
	{
		int xSize = GraphicsSystem::Get()->GetBackBufferWidth();
		int ySize = GraphicsSystem::Get()->GetBackBufferHeight();

		Vector2 pos;

		for (auto p : mObject2s)
		{
			pos = p->position + p->mAABB.center;
			const Klink::JMath::AABB* aabb = &p->mAABB;

			// Max X Bounds
			if(pos.x > xSize - aabb->extend.x)
			{
				p->position.x = xSize - (aabb->extend.x + aabb->center.x);
				p->velocity.x *= -1.0f;
			}
			// Min X Bounds
			else if (pos.x < aabb->extend.x)
			{
				p->position.x = (aabb->extend.x - aabb->center.x);
				p->velocity.x *= -1.0f;
			}
			// Max Y Bounds
			if (pos.y > ySize - aabb->extend.y)
			{
				p->position.y = ySize - (aabb->extend.y + aabb->center.y);
				p->velocity.y *= -1.0f;
			}
			// Min Y Bounds
			else if (pos.y < aabb->extend.y)
			{
				p->position.y = (aabb->extend.y - aabb->center.y);
				p->velocity.y *= -1.0f;
			}
		}
	}


	Object2D* object = nullptr;
	Vector2 objectExtend = Vector2::Zero();
	Vector2 objectCenter = Vector2::Zero();

	Object2D* other = nullptr;
	Vector2 otherExtend = Vector2::Zero();
	Vector2 otherCenter = Vector2::Zero();

	for (size_t objectIndex = 0; objectIndex < mObject2s.size() - 1; ++objectIndex)
	{
		object = mObject2s[objectIndex];
		objectExtend = object->mAABB.extend;
		objectCenter = object->position + object->mAABB.center; // AABB "center" is an offset from the actual position because centers are calculated differently for different collision types

		for (size_t otherIndex = objectIndex + 1; otherIndex < mObject2s.size(); ++otherIndex)
		{
			other = mObject2s[otherIndex];
			otherExtend = other->mAABB.extend;
			otherCenter = other->position + other->mAABB.center;


			float objMinX = objectCenter.x - objectExtend.x;
			float objMaxX = objectCenter.x + objectExtend.x;
			float objMinY = objectCenter.y - objectExtend.y;
			float objMaxY = objectCenter.y + objectExtend.y;

			float otherMinX = otherCenter.x - otherExtend.x;
			float otherMaxX = otherCenter.x + otherExtend.x;
			float otherMinY = otherCenter.y - otherExtend.y;
			float otherMaxY = otherCenter.y + otherExtend.y;

			if (
				objMaxX < otherMinX ||
				objMinX > otherMaxX ||
				objMaxY < otherMinY ||
				objMinY > otherMaxY
				)
			{
				continue;	// NO INTERSECTION
			}
			else
			{
				// INTERSECTION!!
				// Minimum Translation Distance
				Vector2 vec1(std::max(objMinX, otherMinX), std::max(objMinY, otherMinY));
				Vector2 vec2(std::min(objMaxX,otherMaxX), std::min(objMaxY, otherMaxY));

				float distX = vec2.x - vec1.x;
				float distY = vec2.y - vec1.y;
				if (distX < distY)
				{
					// Move by X because it is smaller
					object->position.x += distX;
					object->velocity.x *= -1.0f;
					other->velocity.x *= -1.0f;
				}
				else
				{
					// Move by Y because it is smaller
					object->position.y += distY;
					object->velocity.y *= -1.0f;
					other->velocity.y *= -1.0f;
				}
			}
		}
	}
}

void PhysicsWorld::ResolveOBBCollision(float deltaTime)
{

}