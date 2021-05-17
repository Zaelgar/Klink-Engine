#include "Heightmap.h"

#include "Erosion.h"

using namespace Klink::Terrain;

void Erosion::Initialize(int mapSize)
{
	if (mapSize != mCurrentMapSize || mCurrentErosionRadius != mSettings.erosionRadius)
	{
		InitializeBrushIndices(mapSize, mSettings.erosionRadius);
		mCurrentErosionRadius = mSettings.erosionRadius;
		mCurrentMapSize = mapSize;
	}
}

void Erosion::SetErosionSettings(ErosionSettings settings)
{
	mSettings = settings;
}

void Erosion::Erode(Mesh& mesh, int mapSize, int numIterations)
{
	Vertex* mapHeightValues = mesh.mVertices.data();

	//float heightNW = 0.05f;
	//float heightNE = 0.1f;
	//float heightSW = 0.0f;
	//float heightSE = 0.05f;
	//int x;
	//int y;
	//for (int i = 0; i < mapSize * mapSize; ++i)
	//{
	//	x = i % mapSize;
	//	y = i / mapSize;
	//	mapHeightValues[i].position.y = heightSW * (1 - x) * (1 - y) + heightSE * x * (1 - y) + heightNW * (1 - x) * y + heightNE * x * y;
	//}
	//return;

	for (int iteration = 0; iteration < numIterations; iteration++)
	{
		// Create water droplet at random point on map
		float posX = Klink::JMath::Random::RandomIntUniform(0, mapSize-1);
		float posY = Klink::JMath::Random::RandomIntUniform(0, mapSize-2); // maximum is inclusive, so we make sure we can't go out of bounds
		float dirX = 0;
		float dirY = 0;
		float speed = mSettings.initialSpeed;
		float water = mSettings.initialWaterVolume;
		float sediment = 0;

		for (int lifetime = 0; lifetime < mSettings.maxDropletLifetime; lifetime++)
		{
			float maxDeltaHeight = -1000.0f;

			int nodeX = static_cast<int>(posX);
			int nodeY = static_cast<int>(posY);
			int dropletIndex = nodeY * mapSize + nodeX;
			// Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
			float cellOffsetX = posX - nodeX;
			float cellOffsetY = posY - nodeY;

			// Calculate droplet's height and direction of flow with bilinear interpolation of surrounding heights
			HeightAndGradient heightAndGradient = CalculateHeightAndGradient(mapHeightValues, mapSize, posX, posY);

			// Update the droplet's direction and position (move position 1 unit regardless of speed)
			dirX = (dirX * mSettings.inertia - heightAndGradient.gradientX * (1 - mSettings.inertia));
			dirY = (dirY * mSettings.inertia - heightAndGradient.gradientY * (1 - mSettings.inertia));
			// Normalize direction
			float len = sqrt(dirX * dirX + dirY * dirY);
			ASSERT(!isnan(len), "Invalid SQRT");
			if (len != 0) {
				dirX /= len;
				dirY /= len;
			}
			posX += dirX;
			posY += dirY;

			// Stop simulating droplet if it's not moving or has flowed over edge of map
			if ((dirX == 0 && dirY == 0) || posX < 0 || posX >= mapSize - 1 || posY < 0 || posY >= mapSize - 1)
			{
				break;
			}

			// Find the droplet's new height and calculate the deltaHeight
			float newHeight = CalculateHeightAndGradient(mapHeightValues, mapSize, posX, posY).height;
			float deltaHeight = newHeight - heightAndGradient.height;

			// Calculate the droplet's sediment capacity (higher when moving fast down a slope and contains lots of water)
			float sedimentCapacity = std::max(-deltaHeight * speed * water * mSettings.sedimentCapacityFactor, mSettings.minSedimentCapacity);

			// If carrying more sediment than capacity, or if flowing uphill:
			if (sediment > sedimentCapacity || deltaHeight > 0)
			{
				// If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
				float amountToDeposit = (deltaHeight > 0) ? std::min(deltaHeight, sediment) : (sediment - sedimentCapacity) * mSettings.depositSpeed;
				sediment -= amountToDeposit;

				// Add the sediment to the four nodes of the current cell using bilinear interpolation
				// Deposition is not distributed over a radius (like erosion) so that it can fill small pits
				mapHeightValues[dropletIndex].position.y += amountToDeposit * (1 - cellOffsetX) * (1 - cellOffsetY);
				mapHeightValues[dropletIndex + 1].position.y += amountToDeposit * cellOffsetX * (1 - cellOffsetY);
				mapHeightValues[dropletIndex + mapSize].position.y += amountToDeposit * (1 - cellOffsetX) * cellOffsetY;
				mapHeightValues[dropletIndex + mapSize + 1].position.y += amountToDeposit * cellOffsetX * cellOffsetY;
			}
			else
			{
				// Erode a fraction of the droplet's current carry capacity.
				// Clamp the erosion to the change in height so that it doesn't dig a hole in the terrain behind the droplet
				float amountToErode = std::min((sedimentCapacity - sediment) * mSettings.erodeSpeed, -deltaHeight);

				// Use erosion brush to erode from all nodes inside the droplet's erosion radius
				for (int brushPointIndex = 0; brushPointIndex < mErosionBrushIndices[dropletIndex].size(); brushPointIndex++)
				{
					int nodeIndex = mErosionBrushIndices[dropletIndex][brushPointIndex];
					float weighedErodeAmount = amountToErode * mErosionBrushWeights[dropletIndex][brushPointIndex];
					float deltaSediment = (mapHeightValues[nodeIndex].position.y < weighedErodeAmount) ? mapHeightValues[nodeIndex].position.y : weighedErodeAmount;
					mapHeightValues[nodeIndex].position.y -= deltaSediment;
					sediment += deltaSediment;
				}
			}

			// Update droplet's speed and water content
			if (abs(deltaHeight) > maxDeltaHeight)
				maxDeltaHeight = abs(deltaHeight);

			float prevSpeed = speed;
			float calc = speed * speed + deltaHeight * mSettings.gravity;
			speed = sqrt(std::max(speed * speed + deltaHeight * mSettings.gravity, 0.f));
			ASSERT(!isnan(speed), "Invalid SQRT");
			water *= (1 - mSettings.evaporateSpeed);
		}
	}
}

Erosion::HeightAndGradient Erosion::CalculateHeightAndGradient(Vertex* mapHeightValues, int mapSize, float posX, float posY)
{
	HeightAndGradient heightGradient = {};

	int coordX = static_cast<int>(posX);
	int coordY = static_cast<int>(posY);

	float x = posX - coordX;
	float y = posY - coordY;

	int nodeIndexNW = coordY * mapSize + coordX;
	float heightNW = mapHeightValues[nodeIndexNW].position.y;
	float heightNE = mapHeightValues[nodeIndexNW + 1].position.y;
	float heightSW = mapHeightValues[nodeIndexNW + mapSize].position.y;
	float heightSE = mapHeightValues[nodeIndexNW + mapSize + 1].position.y;

	float gradientX = (heightNE - heightNW) * (1 - y) + (heightSE - heightSW) * y;
	float gradientY = (heightSW - heightNW) * (1 - x) + (heightSE - heightNE) * x;

	// Calculate height with bilinear interpolation of the heights of the nodes of the cell
	float height = heightNW * (1 - x) * (1 - y) + heightNE * x * (1 - y) + heightSW * (1 - x) * y + heightSE * x * y;

	heightGradient.height = height;
	heightGradient.gradientX = gradientX;
	heightGradient.gradientY = gradientY;

	return heightGradient; // RETURN VALUE OPTIMIZATION?
}

void Erosion::InitializeBrushIndices(int mapSize, int radius)
{
	mErosionBrushIndices.resize(mapSize * mapSize);
	mErosionBrushWeights.resize(mapSize * mapSize);

	std::vector<int> xOffsets(radius * radius * 4);
	std::vector<int> yOffsets(radius * radius * 4);
	std::vector<float> weights(radius * radius * 4);
	float weightSum = 0;
	int addIndex = 0;

	for (int i = 0; i < mErosionBrushIndices.size(); ++i)
	{
		int centreX = i % mapSize;
		int centreY = i / mapSize;

		if (centreY <= radius || centreY >= mapSize - radius || centreX <= radius + 1 || centreX >= mapSize - radius)
		{
			weightSum = 0;
			addIndex = 0;
			for (int y = -radius; y <= radius; y++)
			{
				for (int x = -radius; x <= radius; x++)
				{
					float sqrDst = x * x + y * y;
					if (sqrDst < radius * radius)
					{
						int coordX = centreX + x;
						int coordY = centreY + y;

						if (coordX >= 0 && coordX < mapSize && coordY >= 0 && coordY < mapSize)
						{
							float weight = 1 - sqrt(sqrDst) / radius;
							ASSERT(!isnan(weight), "Invalid SQRT");
							weightSum += weight;
							weights[addIndex] = weight;
							xOffsets[addIndex] = x;
							yOffsets[addIndex] = y;
							addIndex++;
						}
					}
				}
			}
		}

		int numEntries = addIndex;
		mErosionBrushIndices[i].resize(numEntries);
		mErosionBrushWeights[i].resize(numEntries);

		for (int j = 0; j < numEntries; j++) {
			mErosionBrushIndices[i][j] = (yOffsets[j] + centreY) * mapSize + xOffsets[j] + centreX;
			mErosionBrushWeights[i][j] = weights[j] / weightSum;
		}
	}
}