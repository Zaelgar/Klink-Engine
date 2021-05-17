#ifndef INCLUDED_GAMESTATE_H
#define INCLUDED_GAMESTATE_H

#include "Common.h"

#include "Sheep.h"
#include "Grass.h"

class GameState : public Klink::AppState
{
public:

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void SpawnSheep();
	void SpawnGrass();

private:

	bool mIsDebugMode = true;

	int mSheepSpawned = 0;
	int mGrassSpawned = 0;
	int mSheepDied = 0;

	int mStartingSheep = 5;
	int mMaxSheep = 30;
	int mStartingGrass = 5;
	int mMaxGrass = 25;

	Camera mCamera;
	TextureID mGrassBackground;
	
	std::vector<Sheep*> mSheeps;
	std::vector<Grass*> mGrasses;
};
#endif // INCLUDED_GAMESTATE_H