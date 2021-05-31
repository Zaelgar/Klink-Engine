#pragma once

#include <Klink/Inc/Klink.h>
#include <vector>

namespace Klink::Terrain
{
	class Erosion
	{
	public:

		struct ErosionSettings
		{
			int erosionRadius = 3;
			float inertia = 0.1f;
			float sedimentCapacityFactor = 6.0f;
			float minSedimentCapacity = 0.01f;
			float erodeSpeed = 0.3f;
			float depositSpeed = 0.3f;
			float evaporateSpeed = 0.01f;
			float gravity = 4.0f;
			int maxDropletLifetime = 30;
			float initialWaterVolume = 1.0f;
			float initialSpeed = 2.8f;
		};

		// Initialize the erosion class by inserting your own ErosionSettings (SetErosionSettings) struct, or leave blank for defaults
		void Initialize(int mapSize);

		void SetErosionSettings(ErosionSettings settings);

		// Function to change height values of a map based on number of water droplet iterations
		void Erode(Klink::Graphics::Mesh& mapHeightValues, int mapSize, int numIterations);


	private:

		struct HeightAndGradient
		{
			float height;
			float gradientX;
			float gradientY;
		};

		HeightAndGradient CalculateHeightAndGradient(Vertex* mapHeightValues, int mapSize, float posX, float posY);
		void InitializeBrushIndices(int mapSize, int radius);


	public:

		ErosionSettings mSettings = {};

		std::vector<std::vector<int>> mErosionBrushIndices;
		std::vector<std::vector<float>> mErosionBrushWeights;

		int mCurrentErosionRadius = 0;
		int mCurrentMapSize = 0;
	};
}