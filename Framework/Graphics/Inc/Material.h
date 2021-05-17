#ifndef INCLUDED_KLINK_GRAPHICS_MATERIAL_H
#define INCLUDED_KLINK_GRAPHICS_MATERIAL_H

#include "Colors.h"

namespace Klink::Graphics
{
struct Material
{
	Colours::Colour ambient;
	Colours::Colour diffuse;
	Colours::Colour specular;
	float power;
	float padding[3];
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_MATERIAL_H