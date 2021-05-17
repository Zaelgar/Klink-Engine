#include "Klink/Inc/Klink.h"


struct Wolf : public Agent
{
	void Initialize()
	{
		steeringModule = std::make_unique<Klink::AI::SteeringModule>(*this);
		steeringModule->AddBehaviour<SeekBehaviour>("Seek")->SetActive(true);
	}

	void Update(float deltaTime)
	{
		// Calculate the push needed for this frame to get us toward the desired vector
		auto force = steeringModule->Calculate();

		// Divide that push by mass in order to make it physics-like
		auto acceleration = force / mass;

		// DeltaTime in order to keep it consistent
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;

		// If moving, update heading to length 1 of velocity
		if (Klink::JMath::Magnitude(velocity) > 0)
		{
			heading = velocity.Normalized();
		}
	}

	void Render()
	{

	}

	StateMachine<Wolf> stateMachine;
	std::unique_ptr<SteeringModule> steeringModule;
};


struct WanderState : public State<Wolf>
{
	static std::string GetName() { return "Dance"; }

	void Enter(Wolf& agent) override
	{
		position = agent.position;
		time = 0.0f;
	}
	void Update(Wolf& agent, float deltaTime) override
	{
		time += deltaTime * 10.0f;
		if (time > Constants::TwoPi)
			agent.stateMachine.ChangeState("Run");
		else
		{
			agent.position.x = position.x + sin(time * 2.0f) * 30.0f;
			agent.position.y = position.y + cos(time * 2.0f) * 10.0f;
		}
	}
	void Exit(Wolf& agent) override
	{
		agent.position = position;
	}

	Klink::JMath::Vector2 position;
	float time;
};