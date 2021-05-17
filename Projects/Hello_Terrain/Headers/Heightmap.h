#pragma once

#include <Klink\Inc\Klink.h>
#include <vector>

namespace Klink::Terrain
{
	class Heightmap
	{
	public:

		// Reads image data where (0,0) is top left, and converts it to (0,0) is bottom left
		Heightmap LoadHeightmapFromImage(const char* fileName);

		bool NormalizeHeightValues();

		void RandomizeHeightValues(float minRandom, float maxRandom);

		std::vector<float> heightmap;
		int xSize = 0;
		int ySize = 0;
	};
};