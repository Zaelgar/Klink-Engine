#ifndef INCLUDED_KLINK_PHYSICS_OBJECT_H
#define INCLUDED_KLINK_PHYSICS_OBJECT_H

namespace Klink::Physics
{

struct Object3D
{
	Klink::JMath::Vector3 lastPosition = Klink::JMath::Vector3::Zero();
	Klink::JMath::Vector3 position = Klink::JMath::Vector3::Zero();
	Klink::JMath::Vector3 velocity = Klink::JMath::Vector3::Zero();
	Klink::JMath::Vector3 acceleration = Klink::JMath::Vector3::Zero();
	float radius = 1.0f;
	float invMass = 0.0f;
	float bounce = 0.0f;

	void SetPosition(const Klink::JMath::Vector3& pos)
	{
		position = pos;
		lastPosition = pos;
	}

	void SetVelocity(const Klink::JMath::Vector3& vel)
	{
		velocity = vel;
		lastPosition = position - vel;
	}
};

struct Object2D
{
public:

	// This function builds a polygon and initializes physics.
	void Initialize(
		int _pointCount,
		float _radius,
		Klink::JMath::Vector2 _position,
		Klink::JMath::Vector2 _velocity,
		Klink::Graphics::Colours::Colour _colour = Klink::Graphics::Colours::YellowGreen);

	void Update(float deltaTime);

	// These functions draw certain aspects of the object, from the polygon to bounding boxes
	void Draw();						// Draw Polygon
	void DrawAABB();					// Draw Axis Aligned Bounding Box	
	void DrawCircleBoundingBox();		// Draw Circle Bounding Box
	void DrawVelocity();				// Visualize velocity
	void ReorderPointsForConvexHull();	// Special function to help with initializing polygon

	void SetPosition(const Klink::JMath::Vector2& pos);
	void SetVelocity(const Klink::JMath::Vector2& vel);

public:
	// Physics and Integration
	Klink::JMath::Vector2 lastPosition = Klink::JMath::Vector2::Zero();
	Klink::JMath::Vector2 position = Klink::JMath::Vector2::Zero();
	Klink::JMath::Vector2 velocity = Klink::JMath::Vector2::Zero();
	Klink::JMath::Vector2 acceleration = Klink::JMath::Vector2::Zero();
	float radius = 1.0f;
	float invMass = 0.0f;
	float bounce = 0.0f;

	

	// Polygon
	std::vector<Klink::JMath::Vector2> mPolygonPoints;
	Klink::JMath::Vector2 mPolygonCenter;
	Klink::Graphics::Colours::Colour colour;

	// Bounding Boxes
	Klink::JMath::AABB mAABB;
	Klink::JMath::Circle mCircleBoundingBox;
};

};	// namespace Klink::Physics

#endif // INCLUDED_KLINK_PHYSICS_PARTICLE_H