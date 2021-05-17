#ifndef INCLUDED_KLINK_PHYSICS_PHYSICSWORLD_H
#define INCLUDED_KLINK_PHYSICS_PHYSICSWORLD_H

namespace Klink::Physics
{
	struct Object2D;
	struct Object3D;

	enum class CollisionType
	{
		CircleBoundingBox
		, AABB
		, OBB
	};

	struct PhysicsSettings
	{
		Klink::JMath::Vector3 gravity{ 0.0f, -9.81, 0.0f };
		float timeStep = 1.0f / 60.0f;
		float drag = 0.0f;
		int iterations = 1;

		int maxParticles = 100;

		bool IsScreenBoundary = false;

		Klink::Physics::CollisionType mCollisionType = Klink::Physics::CollisionType::CircleBoundingBox;
	};

	class PhysicsWorld
	{
	public:
		void Initialize(const PhysicsSettings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugDraw() const;

		void AddObject(Object3D* p);
		void AddObject(Object2D* p);

		void ChangeCollisionType(Klink::Physics::CollisionType collisionType);

		void ClearDynamic(); // HACK

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints(float deltaTime);

		void ResolveCircleBoundCollision(float deltaTime);
		void ResolveAABBCollision(float deltaTime);
		void ResolveOBBCollision(float deltaTime);

		std::vector<Object3D*> mObject3s;
		std::vector<Object2D*> mObject2s;

		PhysicsSettings mSettings;
		float mTimer = 0.0f;
	};

};	// namespace Klink::Physics

#endif // INCLUDED_KLINK_PHYSICS_PHYSICSWORLD_H