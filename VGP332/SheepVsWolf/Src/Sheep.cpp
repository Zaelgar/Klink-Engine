#include "Sheep.h"

using namespace Klink::AI;
using namespace Klink::JMath;

void Sheep::Initialize()
{
	// Initialize State Machine
	mStateMachine = std::make_unique<StateMachine<Sheep>>(*this);

	mStateMachine->AddState<SheepWaitState>();
	mStateMachine->AddState<SheepWanderState>();

	mStateMachine->ChangeState("Wait");

	// Initialize steering
	mSteeringModule = std::make_unique<SteeringModule>(*this);
	mSteeringModule->AddBehaviour<SeekBehaviour>("Seek")->SetActive(true);

	// Initialize Textures
	mSheepWalkUp = TextureManager::Get()->LoadTexture("sheep_up_walk_01.png");
	mSheepWalkRight = TextureManager::Get()->LoadTexture("sheep_right_walk_01.png");
	mSheepWalkLeft = TextureManager::Get()->LoadTexture("sheep_left_walk_01.png");
	mSheepWalkDown = TextureManager::Get()->LoadTexture("sheep_down_walk_01.png");

	// Initialize Movement
	position = RandomScreenPosition(*this);
	mIsMoving = false;

	heading = RandomOnUnitCircle();
}

void Sheep::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);

	if (mIsMoving)
	{
		UpdatePosition(deltaTime);
	}
}

void Sheep::Render()
{
	Vector2 spritePosition = position;
	spritePosition -= Vector2(mSpriteDimensions.widthOffset, mSpriteDimensions.heightOffset);
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(UpdateTexture()), spritePosition);
}

void Sheep::Terminate()
{

}

bool Sheep::UpdatePosition(float deltaTime)
{
	// Calculate the push needed for this frame to get us toward the desired vector
	auto force = mSteeringModule->Calculate();

	// Divide that push by mass in order to make it physics-like
	auto acceleration = force / mass;
	acceleration *= 5.0f;

	// DeltaTime in order to keep it consistent
	velocity += acceleration * deltaTime;
	velocity.Truncate(maxSpeed);


	if (Distance(position, destination) < 15.0f)
	{
		velocity = Vector2::Zero();
		return true;
	}
	position += velocity * deltaTime;

	// If moving, update heading to length 1 of velocity
	if (Klink::JMath::Magnitude(velocity) > 0)
	{
		heading = velocity.Normalized();
	}

	return false;
}

bool Sheep::UpdateHunger(float deltaTime)
{
	mHungerTimeLeft -= deltaTime;

	if (mHungerTimeLeft <= 0)
	{
		mHungerLevel--;

		mHungerTimeLeft = ((rand() % 5000) + 5000);
	}

	if (mHungerLevel <= 0)
	{
		// Returns true, sheep needs to switch to hungry
		return true;
	}

	// no need to switch to hungry
	return false;
}

// Returns a random position based on screen size and size of sprite (grabs a random position where if drawn, is always within the screen limits)
Vector2 Sheep::RandomScreenPosition(Sheep& sheep)
{
	return Vector2
	{
		static_cast<float>(rand() % (GraphicsSystem::Get()->GetBackBufferWidth() - sheep.mSpriteDimensions.width) + sheep.mSpriteDimensions.widthOffset),
		static_cast<float>(rand() % (GraphicsSystem::Get()->GetBackBufferHeight() - sheep.mSpriteDimensions.height) + sheep.mSpriteDimensions.heightOffset)
	};
}

TextureID Sheep::UpdateTexture()
{
	Vector2 base = Vector2(1, -1);
	base.Normalize();

	float angle = atan2(Dot(base, heading), Determinate(base, heading));

	if (angle < 0)
		angle += Constants::TwoPi;

	if (angle < 0)
	{
		angle += Constants::TwoPi;
	}

	if (angle < (Constants::TwoPi * 0.25f) * 1)
	{
		// facing Right
		return mSheepWalkRight;
	}
	if (angle < (Constants::TwoPi * 0.25f) * 2)
	{
		// facing up
		return mSheepWalkUp;
	}
	if (angle < (Constants::TwoPi * 0.25f) * 3)
	{
		// facing left
		return mSheepWalkLeft;
	}
	else
	{
		return mSheepWalkDown;
	}
}