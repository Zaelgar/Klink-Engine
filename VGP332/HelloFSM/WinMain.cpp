#include <AI/Inc/AI.h>
#include <Klink/Inc/Klink.h>

using namespace Klink::AI;
using namespace Klink::JMath;
using namespace Klink::Graphics::SimpleDraw;

struct Dude
{
	Dude() : stateMachine(*this) {}

	Dude(int x, int y)
		: position(x, y)
	{}

	void Initialize()
	{
		stateMachine = std:make_unique<AI::StateMachine<Dude>>(&this);
	}

	StateMachine<Dude> stateMachine;
	Vector2 position;
};

struct DanceState : public State<Dude>
{
	static std::string GetName() { return "Dance"; }

	void Enter(Dude& agent) override
	{
		position = agent.position;
		time = 0.0f;
	}
	void Update(Dude& agent, float deltaTime) override
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
	void Exit(Dude& agent) override
	{
		agent.position = position;
	}

	X::Math::Vector2 position;
	float time;
};

struct RunState : public State<Dude>
{
	static std::string GetName() { return "Run"; }

	void Enter(Dude& agent) override
	{
		destination = RandomUnitCircle() * 250.0f + Vector2{ 600.0f, 350.0f };
	}
	void Update(Dude& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
			agent.stateMachine.ChangeState("Dance");
		else
		{
			agent.position += Normalize(destination - agent.position) * 300.0f * deltaTime;
			DrawScreenLine(agent.position, destination, X::Colors::Red);
			DrawScreenCircle(destination, 10.0f, X::Colors::Green);
		}
	}
	void Exit(Dude& agent) override	{}

	Vector2 destination;
};

Dude dude;
X::TextureId textureId = 0;

void GameInit()
{
	dude.stateMachine.AddState<DanceState>();
	dude.stateMachine.AddState<RunState>();
	textureId = X::LoadTexture("mario.png");
}

void GameCleanup()
{
}

bool GameLoop(float deltaTime)
{
	dude.stateMachine.Update(deltaTime);
	X::DrawSprite(textureId, dude.position);
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}