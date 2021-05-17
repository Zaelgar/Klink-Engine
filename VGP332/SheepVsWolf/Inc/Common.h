#ifndef INCLUDED_SHEEPVSWOLF_COMMON_H
#define INCLUDED_SHEEPVSWOLF_COMMON_H

#include <Klink/Inc/Klink.h>
// TODO - Sprite class (contains texture and Sprite Dimensions for each sprite for rendering)
struct SpriteDimensions
{
	int width;
	int height;
	int widthOffset;
	int heightOffset;
};

enum class GrassQuality
{
	Sprouts,
	Average,
	LongGrass
};



#endif // INCLUDED_SHEEPVSWOLF_COMMON_H