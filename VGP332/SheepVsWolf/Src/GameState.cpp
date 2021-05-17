#include "GameState.h"

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::Input;
using namespace Klink::JMath;

void GameState::Initialize()
{
	srand(time(NULL));
	mGrassBackground = TextureManager::Get()->LoadTexture(L"../../Assets/SheepVsWolf/grassBackground.png");

	// Create sheep starting amount
	for (int i = 0; i < mStartingSheep; i++)
	{
		mSheeps.emplace_back(new Sheep)->Initialize();
	}

	// Create grass starting amount
	for (int i = 0; i < mStartingGrass; i++)
	{
		mGrasses.emplace_back(new Grass)->Initialize(true);
	}
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	static bool isRunning = true;
	{
		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			KlinkApp::Shutdown();
			return;
		}
		if (inputSystem->IsKeyPressed(KeyCode::SPACE))
		{
			isRunning = (isRunning ? false : true);
		}
		if (inputSystem->IsKeyPressed(KeyCode::D))
		{
			mIsDebugMode = (mIsDebugMode ? false : true);
		}
	}

	if (isRunning)
	{
		// Make sure there is always at least minimum grass
		if (mGrasses.size() < mStartingGrass)
		{
			SpawnGrass();
		}

		// Update sheep, check if spawing, and create as necessary
		for (int sheep = 0; sheep < mSheeps.size(); )
		{
			if (mSheeps[sheep] != nullptr)
			{
				mSheeps[sheep]->Update(deltaTime);

				if (mSheeps[sheep]->mHungerLevel <= 1)
				{
					//HACK - possibly add another vector with all positions of grass in it so we dont have to loop
					for (int grass = 0; grass < mGrasses.size(); )
					{
						bool grassDestroyed = false;
						if (mGrasses[grass])
						{
							if (Distance(mGrasses[grass]->position, mSheeps[sheep]->position) <= 30.0f)	// this first if since distance is smaller
							{
								// Eat grass, then destroy it
								mSheeps[sheep]->mHungerLevel += (static_cast<int>(mGrasses[grass]->mQuality) + 1);
								mSheeps[sheep]->mIsSpawning = true;
								
								std::swap(mGrasses[grass], mGrasses[mGrasses.size() - 1]);
								mGrasses.pop_back();
								grassDestroyed = true;
								continue;
								mSheeps[sheep]->mStateMachine->ChangeState("Wander");
							}
							if (Distance(mGrasses[grass]->position, mSheeps[sheep]->position) <= mSheeps[sheep]->mSearchDistance)
							{
								// Found grass, go towards it
								mSheeps[sheep]->destination = mGrasses[grass]->position;
							}
							if(!grassDestroyed)
							{
								++grass;
							}
						}
					}
				}

				// Checks for spawning (2 sheep must be close with hunger level 2 or more)
				if (mSheeps[sheep]->mIsSpawning)
				{
					mSheeps[sheep]->mIsSpawning = false;
					SpawnSheep();
				}

				if (mSheeps[sheep]->mIsDead)
				{
					mSheepDied++;
					// swap and pop
					std::swap(mSheeps[sheep], mSheeps[mSheeps.size() - 1]);
					mSheeps.pop_back();
					continue;
				}
				else
				{
					++sheep;
				}
			}
		}

		// Update grass, check if spreading, and create as necessary
		for (auto& grass : mGrasses)
		{
			if (grass)
			{
				grass->Update(deltaTime);

				// Checks for spreading (1 in 7)
				if (grass->mIsSpreading)
				{
					grass->mIsSpreading = false;
					SpawnGrass();
					break;
				}
			}
		}
	}
}

void GameState::Render()
{
	auto spriteRenderer = SpriteRenderer::Get();
	spriteRenderer->BeginRender();
	spriteRenderer->Draw(*TextureManager::Get()->GetTexture(mGrassBackground), {});

	for (auto& grass : mGrasses)
	{
		if(grass)
		grass->Render();
	}

	// Draw sheep
	for (auto& sheep : mSheeps)
	{
		if(sheep)
		sheep->Render();
	}

	spriteRenderer->EndRender();
}

void GameState::DebugUI()
{
	if (mIsDebugMode)
	{
		for (auto& sheep : mSheeps)
		{
			if (sheep)
			{
				SimpleDraw::DrawScreenCircle(sheep->position, sheep->mSearchDistance);
				SimpleDraw::DrawScreenLine(sheep->position, sheep->position + (sheep->heading * 75.0f), Colours::Aqua);
			}
		}

		for (auto& grass : mGrasses)
		{
			if(grass)
			SimpleDraw::DrawScreenCircle(grass->position, grass->mTimeToWait * 3, Colours::Black);
		}

		ImGui::Text("Sheep Spawned = %i", mSheepSpawned);
		ImGui::Text("Sheep Died = %i", mSheepDied);
		ImGui::Text("Grass Spawned = %i", mGrassSpawned);
	}
	SimpleDraw::Render(mCamera);
}

/*

for (int i = 0; i < size(); )
{
	if (time to die)
	{
		delete vec[i];
		vec[i] = vec.back();
		vec.pop_back();
	}
	else
	{
		i++;
	}
}
*/

void GameState::SpawnSheep()
{
	// Create a new sheep if available
	if (mSheeps.size() < mMaxSheep)
	{
		mSheepSpawned++;
		mSheeps.emplace_back(new Sheep)->Initialize();
	}
}

void GameState::SpawnGrass()
{
	// Create a new grass if available
	if (mGrasses.size() < mMaxGrass)
	{
		mGrassSpawned++;
		mGrasses.emplace_back(new Grass)->Initialize(true);
	}
}