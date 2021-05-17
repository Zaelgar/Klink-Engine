#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include <Klink/Inc/Klink.h>

class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	// General
	Klink::Graphics::Camera mCamera;
	Klink::Physics::PhysicsWorld mPhysicsWorld;

	// Particles
	int minimumPoints = 4;
	int maximumPoints = 7;
	std::vector<Klink::Physics::Object2D> mObjects;

	int mObjectCount = 2;
	float mMinimumRadius = 150.f;
	float mMaximumRadius = 200.f;

	float mMinimumSpeed = 250.0f;
	float mMaximumSpeed = 300.0f;

	// Demo
	bool isSphereCollision = true;
	bool isBoxCollision = false;

	// Debug
	bool isPaused = true;
	bool isDrawingAABB = false;
	bool isDrawingCircleBoundingBox = true;
	bool isDrawingVelocity = false;
};
#endif // INCLUDED_GAMESTATE_H