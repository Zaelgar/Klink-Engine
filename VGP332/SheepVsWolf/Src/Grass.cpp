#include "Grass.h"

void Grass::Initialize(bool isActive)
{
	// Initialize State Machine
	mStateMachine = std::make_unique<StateMachine<Grass>>(*this);

	mStateMachine->AddState<GrassWaitState>();
	mStateMachine->AddState<GrassGrowState>();

	mStateMachine->ChangeState("Wait");

	mSprouts = TextureManager::Get()->LoadTexture("grass_sprouts_01.png");
	mAverage = TextureManager::Get()->LoadTexture("grass_average_01.png");
	mLongGrass = TextureManager::Get()->LoadTexture("grass_longGrass_01.png");

	mIsGrowing = isActive;

	position = SpawnGrass(*this);
}

void Grass::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}

void Grass::Terminate()
{
}

void Grass::Render()
{
	Vector2 spritePosition = position;
	spritePosition -= Vector2(mSpriteDimensions.widthOffset, mSpriteDimensions.heightOffset);
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(UpdateTexture()), spritePosition);
}

Klink::JMath::Vector2 Grass::SpawnGrass(Grass& grass)
{
	return Vector2
	{
		static_cast<float>(rand() % (GraphicsSystem::Get()->GetBackBufferWidth() - grass.mSpriteDimensions.width) + grass.mSpriteDimensions.widthOffset),
		static_cast<float>(rand() % (GraphicsSystem::Get()->GetBackBufferHeight() - grass.mSpriteDimensions.height) + grass.mSpriteDimensions.heightOffset)
	};
}

TextureID Grass::UpdateTexture()
{
	if (mQuality == GrassQuality::Sprouts)
		return mSprouts;
	if (mQuality == GrassQuality::Average)
		return mAverage;
	else
		return mLongGrass;
}