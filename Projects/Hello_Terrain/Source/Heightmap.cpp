#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Heightmap.h"

#include <limits>
#include <array>

using namespace Klink::Terrain;

// Reads in data
Heightmap Heightmap::LoadHeightmapFromImage(const char* fileName)
{
	Heightmap hm;

	int n;
	unsigned char* data = nullptr;

	data = stbi_load(fileName, &hm.xSize, &hm.ySize, &n, 1); // Don't forget to unload data (pure black pixels are /0 ??)
	ASSERT(data != nullptr, "Image failed to load.");

	std::vector<float> values;
	values.resize(hm.xSize * hm.ySize);

	// x = 0, y = 0 is top left of the image values
	for (int i = 0; i < hm.xSize * hm.ySize; ++i)
	{
		values[i] = static_cast<float>(data[i]);
	}

	hm.heightmap.resize(hm.xSize * hm.ySize);
	int index = 0;
	int i = 0;
	for (int y = hm.ySize-1; y >= 0; --y)
	{
		for (int x = 0; x < hm.xSize; ++x)
		{
			index = y * hm.ySize + x;
			hm.heightmap[i++] = values[index];
		}
	}

	stbi_image_free(data); // Data unload

	return hm;
}

bool Klink::Terrain::Heightmap::NormalizeHeightValues()
{
	if (heightmap.empty())
		return false;

	float min = std::numeric_limits<float>::max();
	float max = std::numeric_limits<float>::min();
	for (float height : heightmap)
	{
		max = std::max(max, height);
		min = std::min(min, height);
	}

	if (max != min)
	{
		for (float& height : heightmap)
		{
			height = (height - min) / (max - min);
		}
		return true;
	}

	return false;
}

void Klink::Terrain::Heightmap::RandomizeHeightValues(float minRandom = -0.005f, float maxRandom = 0.005f)
{
	for (float& height : heightmap)
	{
		height += Klink::JMath::Random::RandomFloatUniform(minRandom, maxRandom);
	}
}

Heightmap Klink::Terrain::Heightmap::GeneratePerlinHeightmap(int size)
{
	Heightmap hm;



	return hm;
}