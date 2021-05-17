#include <Klink/Inc/Klink.h>



class Sheep : public Agent2D
{
public:

	Sheep(AIWorld& world);

	void Initialize();
	void Update(float deltaTime);
	void Render();
	void Terminate();

	static Klink::JMath::Vector2 RandomScreenPosition(Sheep& sheep);
	
	// Updates Sheep position. If it has arrived at the destination, returns true
	bool UpdatePosition(float deltaTime);
	// Some function needed to select which texture needs to be rendered
	TextureID UpdateTexture();
	// Function to randomly select and update mHungerLevel
	bool UpdateHunger(float deltaTime);

	// Game variables
	bool mIsMoving = false;
	bool mIsSpawning = false;
	bool mIsDead = false;

	float mSearchDistance = 200.0f;
	int mHungerLevel = 2;
	float mHungerTimeLeft = 20.0f;

	std::unique_ptr<StateMachine<Sheep>> mStateMachine;
	std::unique_ptr<SteeringModule> mSteeringModule;
	std::unique_ptr<PerceptionModule> mPerceptionModule;

	SpriteDimensions mSpriteDimensions{49, 62, (49/2), (62/2)};
	Sprite mSprite;
};

// ---------------------------------------------------

struct SheepSimpleWanderState : public State<Sheep>
{
	static std::string GetName() { return "SimpleWander"; }

	void Enter(Sheep& agent) override
	{
		agent.destination = Sheep::RandomScreenPosition(agent);
	}
	void Update(Sheep& agent, float deltaTime) override
	{
		if (Vector2::Distance(agent.position, agent.destination) <= 20.0f )
		{
			agent.position = Sheep::RandomScreenPosition(agent);
		}
	}
	void Exit(Sheep& agent) override
	{}
};

struct SheepWanderState : public State<Sheep>
{
	static std::string GetName() { return "Wander"; }

	void Enter(Sheep& agent) override
	{
		agent.destination = Sheep::RandomScreenPosition(agent);
	}
	void Update(Sheep& agent, float deltaTime) override
	{
		agent.mHungerTimeLeft -= deltaTime;
		if (agent.mHungerTimeLeft <= 0.0f)
		{
			agent.mHungerTimeLeft = (rand() % 10) + 15;
			agent.mHungerLevel--;
			if (agent.mHungerLevel < 0)
				agent.mIsDead = true;
		}

		if (agent.velocity == Vector2::Zero())
		{
			agent.mStateMachine->ChangeState("Wait");
		}
	}
	void Exit(Sheep& agent) override
	{}
};

// ---------------------------------------------------

struct SheepWaitState : public State<Sheep>
{
	static std::string GetName() { return "Wait"; }

	void Enter(Sheep& agent) override
	{
		agent.mIsMoving = false;
		
		// Set wait time between 2-5 seconds
		timeToWait = static_cast<float>((rand() % 2) + 3.0f);
	}
	void Update(Sheep& agent, float deltaTime) override
	{
		// Check for hunger
		agent.mHungerTimeLeft -= deltaTime;
		if (agent.mHungerTimeLeft <= 0.0f)
		{
			agent.mHungerTimeLeft = ((rand() % 10) + 10);
			if (--agent.mHungerLevel)
				agent.mIsDead = true;
		}

		// Check for waiting time up
		timeToWait -= deltaTime;
		if (timeToWait <= 0.0f)
			agent.mStateMachine->ChangeState("Wander");
	}
	void Exit(Sheep& agent) override
	{
		agent.mIsMoving = true;
	}

	float timeToWait;
};