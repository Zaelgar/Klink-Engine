#ifndef INCLUDED_KLINK_GRAPHICS_VERTEXTYPES_H
#define INCLUDED_KLINK_GRAPHICS_VERTEXTYPES_H

#include "Common.h"
#include "Colors.h"

namespace Klink::Graphics
{
	// Vertex Element Flags
	constexpr uint32_t VE_Position		= 0x1 << 0;
	constexpr uint32_t VE_Normal		= 0x1 << 1;
	constexpr uint32_t VE_Tangent		= 0x1 << 2;
	constexpr uint32_t VE_Color			= 0x1 << 3;
	constexpr uint32_t VE_Texcoord		= 0x1 << 4;
	constexpr uint32_t VE_BlendIndex	= 0x1 << 5;
	constexpr uint32_t VE_BlendWeights	= 0x1 << 6;

#define VERTEX_FORMAT(fmt)\
	static constexpr uint32_t Format = fmt

	struct VertexPC // Position, Colour
	{
		VERTEX_FORMAT(VE_Position | VE_Color);

		VertexPC() = default;

		constexpr VertexPC(const JMath::Vector3& Position, const Klink::Graphics::Colours::Colour& colour)
			: position(Position),
			color(colour)
		{}

		JMath::Vector3 position;
		Klink::Graphics::Colours::Colour color;
	};

	struct VertexPN
	{
		VERTEX_FORMAT(VE_Position | VE_Normal);

		VertexPN() = default;

		constexpr VertexPN(const JMath::Vector3& Position, const JMath::Vector3& Normal)
			: position(Position)
			, normal(Normal)
		{}

		JMath::Vector3 position;
		JMath::Vector3 normal;
	};



	struct VertexPNX // Position, Normal, Texture
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Texcoord);

		VertexPNX() = default;

		constexpr VertexPNX(const JMath::Vector3& Position, const JMath::Vector3& Normal, const JMath::Vector2& UV)
			: position(Position),
			normal(Normal),
			u(UV.x),
			v(UV.y)
		{}

		constexpr VertexPNX(const JMath::Vector3& Position, const JMath::Vector3& Normal, const float U, const float V)
			: position(Position),
			normal(Normal),
			u(U),
			v(V)
		{}

		JMath::Vector3 position;
		JMath::Vector3 normal;
		float u, v;
	};

	struct VertexPX // Position Texture
	{
		VERTEX_FORMAT(VE_Position | VE_Texcoord);

		VertexPX() = default;

		constexpr VertexPX(const JMath::Vector3& Position, const JMath::Vector2& UV)
			: position(Position),
			u(UV.x),
			v(UV.y)
		{}

		constexpr VertexPX(const JMath::Vector3& Position, const float U, const float V)
			: position(Position),
			u(U),
			v(V)
		{}

		JMath::Vector3 position;
		float u, v;
	};

	struct Vertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_Texcoord);

		JMath::Vector3 position;
		JMath::Vector3 normal;
		JMath::Vector3 tangent;
		JMath::Vector2 UV;
	};

	struct BoneVertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_Texcoord | VE_BlendIndex | VE_BlendWeights);

		JMath::Vector3 position;
		JMath::Vector3 normal;
		JMath::Vector3 tangent;
		JMath::Vector2 UV;
		int boneIndices[4] = { 0 };
		float boneWeights[4] = { 0.0f };

	};

} // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_TYPES_H