#include "Klink/Inc/Klink.h"
#include "Fish.h"

using namespace Klink::AI;
using namespace Klink::JMath;

Fish::Fish(AIWorld& _world)
	:Agent2D(_world)
{}

void Fish::Initialize()
{
	agentType = Agent2D::AgentType::Fish;

	steeringModule = std::make_unique<Klink::AI::SteeringModule>(*this);
	//steeringModule->AddBehaviour<WanderBehaviour>("3Wander", 3)->SetActive(true);
	//steeringModule->AddBehaviour<SeekBehaviour>("3Seek", 3)->SetActive(true);
	//steeringModule->AddBehaviour<ArriveBehaviour>("3Arrive", 3)->SetActive(true);
	steeringModule->AddBehaviour<ObstacleAvoidanceBehaviour>("2ObstacleAvoidance", 2)->SetActive(true);
	steeringModule->AddBehaviour<WallAvoidanceBehaviour>("1WallAvoidance", 1)->SetActive(true);
	//steeringModule->AddBehaviour<FleeBehaviour>("3Flee", 3)->SetActive(true);

	/// Flocking
	steeringModule->AddBehaviour<SeparationBehaviour>("3FlockingSeparation", 3)->SetActive(true);
	steeringModule->AddBehaviour<AlignmentBehaviour>("3FlockingAlignment", 3)->SetActive(true);
	steeringModule->AddBehaviour<CohesionBehaviour>("3FlockingCohesion", 3)->SetActive(true);

	radius = 20.0f;

	mTriangle.pointOne =	{radius, 0.0f};
	mTriangle.pointTwo =	{0.0f, radius * 0.5f};
	mTriangle.pointThree =	{0.0f, -radius * 0.5f};
}

void Fish::Terminate()
{
}

void Fish::Update(float deltaTime, bool b)
{
	if (b)
	{
		auto force = steeringModule->Calculate();
		auto acceleration = force / mass;
		velocity += acceleration * deltaTime;
		velocity.Truncate(maxSpeed);
		position += velocity * deltaTime;

		if (Vector2::Magnitude(velocity) > 0)
		{
			heading = velocity.Normalized();
		}
	}
}

void Fish::Render(Klink::Graphics::Colours::Colour color)
{
	if (isDebugging)
	{
		SimpleDraw::DrawScreenSquare(destination, 5.0f, Colours::Cornsilk);
		SimpleDraw::DrawScreenLine(position, position + (heading * maxSpeed), Colours::BurlyWood);
	}

	SimpleDraw::DrawScreenTriangle(Matrix3::TransformCoord(mTriangle.pointOne, LocalToWorld()), Matrix3::TransformCoord(mTriangle.pointTwo, LocalToWorld()), Matrix3::TransformCoord(mTriangle.pointThree, LocalToWorld()), color);
}

Vector2 Fish::RandomPositionOnScreen()
{
	int heightMax = GraphicsSystem::Get()->GetBackBufferHeight() - (radius * 2.0f);
	int heightMin = radius;
	int widthMax = GraphicsSystem::Get()->GetBackBufferWidth() - (radius * 2.0f);
	int widthMin = radius;

	int xPosition = (rand() % widthMax) + widthMin;
	int yPosition = (rand() % heightMax) + heightMin;

	return Vector2( xPosition, yPosition );
}