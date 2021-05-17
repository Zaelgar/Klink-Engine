#ifndef INCLUDED_ENGINE_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_ENGINE_GRAPHICS_SIMPLEDRAW_H

#include "Colors.h"

namespace Klink::Graphics { class Camera; }
namespace Klink::Graphics::SimpleDraw
{

void StaticInitialize(uint32_t maxVertexCount = 10000);
void StaticTerminate();

void DrawDebugGrid(int lineCount = 100, const Klink::Graphics::Colours::Colour color = Colours::Black);
void DrawScreenDebugGrid(int lineSpacing = 100, const Klink::Graphics::Colours::Colour color = Colours::DarkSlateGray);
void DrawLine(const Klink::JMath::Vector3& v0, const Klink::JMath::Vector3& v1, const Klink::Graphics::Colours::Colour color = Colours::Magenta);

void DrawSquare(const Klink::JMath::Vector3& center, float sideLength, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawCircle(const Klink::JMath::Vector3& center, float radius, const Klink::Graphics::Colours::Colour color = Colours::Magenta, int sideCount = 16);
void DrawCube(const Klink::JMath::Vector3& center, float sideLength, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawSphere(const Klink::JMath::Vector3 & center, float radius, const Klink::Graphics::Colours::Colour color = Colours::Magenta, uint32_t sliceCount = 16, uint32_t ringCount = 16);

void DrawScreenLine(const Klink::JMath::Vector2& v0, const Klink::JMath::Vector2& v1, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenLine(const Klink::JMath::LineSegment& line, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenSquare(const Klink::JMath::Vector2& center, float sideLength, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenRect(const Klink::JMath::Vector2& topLeft, const Klink::JMath::Vector2& bottomRight, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenRect(const Klink::JMath::Vector2& topLeft, const Klink::JMath::Vector2& topRight, const Klink::JMath::Vector2& bottomLeft, const Klink::JMath::Vector2& bottomRight, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenCircle(const Klink::JMath::Vector2& center, float radius, const Klink::Graphics::Colours::Colour color = Colours::Magenta);
void DrawScreenTriangle(const Klink::JMath::Vector2& tip, const Klink::JMath::Vector2& left, const Klink::JMath::Vector2& right, const Klink::Graphics::Colours::Colour color = Colours::Magenta);

void Render(const Camera& camera);

} // namespace Engine::Graphics::SimpleDraw

#endif // #ifndef INCLUDED_ENGINE_GRAPHICS_SIMPLEDRAW_H