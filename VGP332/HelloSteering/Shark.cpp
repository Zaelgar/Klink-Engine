#include "Klink/Inc/Klink.h"
#include "Shark.h"

using namespace Klink::AI;
using namespace Klink::JMath;

Shark::Shark(AIWorld& _world)
	:Agent2D(_world)
{}

void Shark::Initialize()
{
	agentType = Agent2D::AgentType::Shark;

	steeringModule = std::make_unique<Klink::AI::SteeringModule>(*this);
	steeringModule->AddBehaviour<SeekBehaviour>("Seek", 3)->SetActive(true);
	steeringModule->AddBehaviour<PursuitBehaviour>("Pursuit", 3)->SetActive(true);
	steeringModule->AddBehaviour<ArriveBehaviour>("Arrive", 3)->SetActive(true);
	steeringModule->AddBehaviour<WanderBehaviour>("Wander", 3)->SetActive(false);
	steeringModule->AddBehaviour<ObstacleAvoidanceBehaviour>("ObstacleAvoidance", 2)->SetActive(true);
	steeringModule->AddBehaviour<WallAvoidanceBehaviour>("WallAvoidance", 2)->SetActive(true);
}

void Shark::Terminate()
{
}

void Shark::Update(float deltaTime)
{
	auto force = steeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;

	if (Vector2::Magnitude(velocity) > 0)
	{
		heading = velocity.Normalized();
	}
}

void Shark::Render(Klink::Graphics::Colours::Colour color)
{
	SimpleDraw::DrawScreenSquare(destination, 5.0f, Colours::Chartreuse);

	SimpleDraw::DrawScreenLine(position, position + (heading * 30.0f), Colours::BurlyWood);
	SimpleDraw::DrawScreenCircle(position, radius, color);
}