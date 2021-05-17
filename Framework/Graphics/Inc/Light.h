#ifndef INCLUDED_KLINK_GRAPHICS_LIGHT_H
#define INCLUDED_KLINK_GRAPHICS_LIGHT_H

#include "Colors.h"

namespace Klink::Graphics
{

struct PointLight
{
	JMath::Vector3 position;
	float padding;
	Colours::Colour ambient;
	Colours::Colour diffuse;
	Colours::Colour specular;
};

struct DirectionalLight
{
	JMath::Vector3 direction;
	float padding;
	Colours::Colour ambient;
	Colours::Colour diffuse;
	Colours::Colour specular;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_LIGHT_H