#ifndef INCLUDED_SHEEPVSWOLF_GRASS_H
#define INCLUDED_SHEEPVSWOLF_GRASS_H

#include "Common.h"

class Grass
{
public:

	void Initialize(bool isActive);
	void Update(float deltaTime);
	void Terminate();
	void Render();

	Klink::JMath::Vector2 SpawnGrass(Grass& grass);
	TextureID UpdateTexture();
	
	Klink::JMath::Vector2 position;

	bool mIsGrowing = false;
	bool mIsSpreading = false;

	float mTimeToWait = 0.0f;

	GrassQuality mQuality{ GrassQuality::Sprouts };

	SpriteDimensions mSpriteDimensions{ 54, 52, (54 / 2), (52 / 2) };

	TextureID mSprouts;
	TextureID mAverage;
	TextureID mLongGrass;

	std::unique_ptr<StateMachine<Grass>> mStateMachine;
};

struct GrassWaitState : public State<Grass>
{
	static std::string GetName() { return "Wait"; }

	void Enter(Grass& agent) override
	{
		agent.mTimeToWait = ((rand() % 10) + 10);
	}
	void Update(Grass& agent, float deltaTime) override
	{
		if (static_cast<Grass&>(agent).mIsGrowing)
		{
			agent.mTimeToWait -= deltaTime;
			if (agent.mTimeToWait <= 0)
			{
				static_cast<Grass&>(agent).mStateMachine->ChangeState("Grow");
			}
		}
	}
	void Exit(Grass& agent) override
	{
	}
};

struct GrassGrowState : public State<Grass>
{
	static std::string GetName() { return "Grow"; }

	void Enter(Grass& agent) override
	{
		if (static_cast<Grass&>(agent).mQuality == GrassQuality::Sprouts)
			static_cast<Grass&>(agent).mQuality = GrassQuality::Average;
		
		else if (static_cast<Grass&>(agent).mQuality == GrassQuality::Average)
			static_cast<Grass&>(agent).mQuality = GrassQuality::LongGrass;
		
		else
			static_cast<Grass&>(agent).mIsGrowing = false;

		agent.mStateMachine->ChangeState("Wait");
	}
	void Update(Grass& agent, float deltaTime) override
	{}
	void Exit(Grass& agent) override
	{
		// one in 7 chance to spread when it grows
		agent.mIsSpreading = !(rand() % 7);
	}
};

#endif // INCLUDED_SHEEPVSWOLF_GRASS_H