#ifndef INCLUDED_KLINK_GRAPHICS_KEYFRAME_H
#define INCLUDED_KLINK_GRAPHICS_KEYFRAME_H

#include "JMath/Inc/JMath.h"

namespace Klink::Graphics
{
template <typename KeyType>
struct Keyframe
{
	Keyframe(KeyType v, float t)
		: value(v)
		, time(t)
	{}

	KeyType value;
	float time;
};

} // namespace Klink::Graphics
#endif // INCLUDED_KLINK_GRAPHICS_KEYFRAME_H