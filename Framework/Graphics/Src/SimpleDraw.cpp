#include "Precompiled.h"
#include "SimpleDraw.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"
#include "Colors.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"


using namespace Klink::Graphics;
using namespace Klink::JMath;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Colours::Colour color);
		void AddScreenLine(const Vector2& v0, const Vector2& v1, const Colours::Colour color);
		//void AddFace(const Vector3& v0, const Vector3& v1, const Colours::Colour& color);


		void Render3D(const Camera& camera);
		void Render2D(const Camera& camera);


	private:

		bool mIsInitialized{ false };

		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		MeshBuffer mMeshBuffer;

		std::vector<VertexPC> mVertices3D;
		std::vector<VertexPC> mVertices2D;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		ASSERT(!mIsInitialized, "[SimpleDraw] Already initialized!");

		mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.Initialize(maxVertexCount, sizeof(VertexPC), nullptr, true);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mVertices3D.reserve(maxVertexCount);
		mVertices2D.reserve(maxVertexCount);

		mIsInitialized = true;
	}

	void SimpleDrawImpl::Terminate()
	{
		ASSERT(mIsInitialized, "[SimpleDraw] Already terminated!");

		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mConstantBuffer.Terminate();
		mMeshBuffer.Terminate();

		mIsInitialized = false;
	}

	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Colours::Colour color)
	{
		ASSERT(mIsInitialized, "[SimpleDraw] Not initialized.");

		if (mVertices3D.size() + 2 <= mVertices3D.capacity())
		{
			mVertices3D.emplace_back(VertexPC{ v0, color });
			mVertices3D.emplace_back(VertexPC{ v1, color });
		}
	}

	void SimpleDrawImpl::AddScreenLine(const Vector2 & v0, const Vector2 & v1, const Colours::Colour color)
	{
		ASSERT(mIsInitialized, "[SimpleDraw] Not initialized.");

		if (mVertices2D.size() + 2 <= mVertices2D.capacity())
		{
			mVertices2D.emplace_back(VertexPC{ {v0.x, v0.y, 0.0f}, color });
			mVertices2D.emplace_back(VertexPC{ {v1.x, v1.y, 0.0f}, color });
		}
	}

	void SimpleDrawImpl::Render3D(const Camera& camera)
	{
		ASSERT(mIsInitialized, "[SimpleDraw] Not initialized.");
		if (mVertices3D.empty())
		{
			return;
		}

		Matrix4 matView = camera.GetViewMatrix();
		Matrix4 matProj = camera.GetPerspectiveMatrix();

		mVertexShader.Bind();
		mPixelShader.Bind();

		Matrix4 transform = Matrix4::Transpose(matView * matProj);

		mConstantBuffer.Set(&transform);
		mConstantBuffer.BindVS();

		mMeshBuffer.Update(mVertices3D.size(), mVertices3D.data());

		mMeshBuffer.Render();

		mVertices3D.clear();
	}

	void SimpleDrawImpl::Render2D(const Camera& camera)
	{
		ASSERT(mIsInitialized, "[SimpleDraw] Not initialized.");
		
		if (mVertices2D.empty())
		{
			return;
		}

		mVertexShader.Bind();
		mPixelShader.Bind();

		Matrix4 screenSpace
		{
			2.0f / GraphicsSystem::Get()->GetBackBufferWidth(), 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / GraphicsSystem::Get()->GetBackBufferHeight(), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};

		screenSpace = screenSpace.Transpose();

		mConstantBuffer.Set(&screenSpace);
		mConstantBuffer.BindVS();

		mMeshBuffer.Update(mVertices2D.size(), mVertices2D.data());

		mMeshBuffer.Render();

		mVertices2D.clear();
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::DrawSquare(const Klink::JMath::Vector3& center, float sideLength, const Klink::Graphics::Colours::Colour color)
{
	float halfLength = sideLength / 2.0f;
	// Top
	DrawLine({ -halfLength + center.x,  halfLength + center.y, center.z }, { halfLength + center.x,  halfLength + center.y, center.z }, color);
	// Right
	DrawLine({ halfLength + center.x,  halfLength + center.y, center.z }, { halfLength + center.x, -halfLength + center.y, center.z }, color);
	// Bottom
	DrawLine({ halfLength + center.x, -halfLength + center.y, center.z }, { -halfLength + center.x, -halfLength + center.y, center.z }, color);
	// Left
	DrawLine({ -halfLength + center.x, -halfLength + center.y, center.z }, { -halfLength + center.x,  halfLength + center.y, center.z }, color);
}

void SimpleDraw::DrawCube(const Klink::JMath::Vector3& center, float sideLength, const Klink::Graphics::Colours::Colour color)
{
	float halfLength = sideLength / 2.0f;
	// Front
	DrawSquare({ center.x, center.y, center.z - halfLength }, sideLength, color);
	// Back
	DrawSquare({ center.x, center.y, center.z + halfLength }, sideLength, color);

	// Top Left
	DrawLine({ center.x - halfLength, center.y + halfLength, center.z - halfLength }, { center.x - halfLength, center.y + halfLength, center.z + halfLength }, color);
	// Top Right
	DrawLine({ center.x + halfLength, center.y + halfLength, center.z - halfLength }, { center.x + halfLength, center.y + halfLength, center.z + halfLength }, color);
	// Bottom Right
	DrawLine({ center.x + halfLength, center.y - halfLength, center.z - halfLength }, { center.x + halfLength, center.y - halfLength, center.z + halfLength }, color);
	// Botom Left
	DrawLine({ center.x - halfLength, center.y - halfLength, center.z - halfLength }, { center.x - halfLength, center.y - halfLength, center.z + halfLength }, color);
}

void SimpleDraw::DrawSphere(const Klink::JMath::Vector3 & center, float radius, const Klink::Graphics::Colours::Colour color, uint32_t sliceCount, uint32_t ringCount)
{
	ASSERT((sliceCount >= 3 && ringCount >= 3), "[SimpleDraw::DrawSphere] Slices or Rings size does not meet minimum requirements!");

	// ringTheta is the angle between each ring (based on number of ringCount) going around a half circle
	const float ringTheta = JMath::Constants::Pi / (float)ringCount;
	// slicePhi is the angle between each slice (based on number of sliceCount) going around the full circle
	const float slicePhi = JMath::Constants::TwoPi / (float)sliceCount;

	// For each slice (meridians, vertical, orange slice), we draw the lines for each slice, and each ring between a slice and its neighbour slice
	for (uint32_t currentSlice = 0; currentSlice < sliceCount; ++currentSlice)
	{
		for (uint32_t currentRing = 0; currentRing < ringCount; ++currentRing)
		{
			const float startAngle = currentRing * ringTheta;
			const float neighbourAngle = startAngle + ringTheta;
			const float ay = radius * cos(startAngle);
			const float by = radius * cos(neighbourAngle);

			const float theta = currentSlice * slicePhi;
			const float phi = theta + slicePhi;

			const float ar = sqrt(radius * radius - ay * ay);
			const float br = sqrt(radius * radius - by * by);

			const float x0 = center.x + (ar * sin(theta));
			const float y0 = center.y + (ay);
			const float z0 = center.z + (ar * cos(theta));

			const float x1 = center.x + (br * sin(theta));
			const float y1 = center.y + (by);
			const float z1 = center.z + (br * cos(theta));

			const float x2 = center.x + (br * sin(phi));
			const float y2 = center.y + (by);
			const float z2 = center.z + (br * cos(phi));

			DrawLine({ x0, y0, z0 }, {x1, y1, z1}, color);

			if (currentRing < ringCount - 1)
			{
				DrawLine({ x1, y1, z1 }, { x2, y2, z2 }, color);
			}
		}
	}
}

void SimpleDraw::DrawScreenLine(const Klink::JMath::Vector2 & v0, const Klink::JMath::Vector2 & v1, const Klink::Graphics::Colours::Colour color)
{
	sInstance->AddScreenLine(v0, v1, color);
}

void SimpleDraw::DrawScreenLine(const Klink::JMath::LineSegment& line, const Klink::Graphics::Colours::Colour color)
{
	sInstance->AddScreenLine(line.from, line.to, color);
}

void SimpleDraw::DrawScreenSquare(const Klink::JMath::Vector2 & center, float sideLength, const Klink::Graphics::Colours::Colour color)
{
	float halfLength = sideLength / 2.0f;

	// Top
	DrawScreenLine({ -halfLength + center.x,  halfLength + center.y }, { halfLength + center.x,  halfLength + center.y }, color);
	// Right
	DrawScreenLine({ halfLength + center.x,  halfLength + center.y }, { halfLength + center.x, -halfLength + center.y }, color);
	// Bottom
	DrawScreenLine({ halfLength + center.x, -halfLength + center.y }, { -halfLength + center.x, -halfLength + center.y }, color);
	// Left
	DrawScreenLine({ -halfLength + center.x, -halfLength + center.y }, { -halfLength + center.x,  halfLength + center.y }, color);
}

void SimpleDraw::DrawScreenRect(const Klink::JMath::Vector2 & topLeft, const Klink::JMath::Vector2 & bottomRight, const Klink::Graphics::Colours::Colour color)
{
	// Top
	DrawScreenLine(topLeft, { bottomRight.x, topLeft.y }, color);
	// Right
	DrawScreenLine({bottomRight.x, topLeft.y}, bottomRight, color);
	// Bot
	DrawScreenLine(bottomRight, { topLeft.x, bottomRight.y }, color);
	// Left
	DrawScreenLine({topLeft.x, bottomRight.y}, topLeft, color);
}

void SimpleDraw::DrawScreenRect(const Klink::JMath::Vector2 & topLeft, const Klink::JMath::Vector2 & topRight, const Klink::JMath::Vector2 & bottomLeft, const Klink::JMath::Vector2 & bottomRight, const Klink::Graphics::Colours::Colour color)
{
	DrawScreenLine(topLeft, topRight, color);

	DrawScreenLine(topRight, bottomRight, color);

	DrawScreenLine(bottomRight, bottomLeft, color);

	DrawScreenLine(bottomLeft, topLeft, color);
}

void Klink::Graphics::SimpleDraw::DrawScreenCircle(const Klink::JMath::Vector2 & center, float radius, const Klink::Graphics::Colours::Colour color)
{
	int sideCount = 24;
	float x = center.x;
	float y = center.y;

	// Just like in mesh builder, slice it up into sections based on number of sides, then draw lines between the start and end for each slice
	const float sliceAngle = (2.0f * JMath::Constants::Pi) / sideCount;
	for (int i = 0; i < sideCount; ++i)
	{
		const float theta = i * sliceAngle;
		const float thetaNext = theta + sliceAngle;
		const float x0 = x + (radius * sin(theta));
		const float y0 = y + (radius * cos(theta));
		const float x1 = x + (radius * sin(thetaNext));
		const float y1 = y + (radius * cos(thetaNext));

		DrawScreenLine({ x0, y0 }, { x1, y1 }, color);
	}
}

void SimpleDraw::DrawCircle(const Klink::JMath::Vector3 & center, float radius, const Colours::Colour color, int sideCount)
{
	ASSERT((sideCount >= 3), "[SimpleDraw::DrawCircle] Side count must be more than 2!");
	if (sideCount < 3)
	{
		sideCount = 3;
	}

	float x = center.x;
	float y = center.y;

	// Just like in mesh builder, slice it up into sections based on number of sides, then draw lines between the start and end for each slice
	const float sliceAngle = (2.0f * JMath::Constants::Pi) / sideCount;
	for (int i = 0; i < sideCount; ++i)
	{
		const float theta = i * sliceAngle;
		const float thetaNext = theta + sliceAngle;
		const float x0 = x + (radius * sin(theta));
		const float y0 = y + (radius * cos(theta));
		const float x1 = x + (radius * sin(thetaNext));
		const float y1 = y + (radius * cos(thetaNext));

		DrawLine({ x0, y0, center.z }, { x1, y1, center.z }, color);
	}
}

void SimpleDraw::DrawScreenTriangle(const Klink::JMath::Vector2& tip, const Klink::JMath::Vector2& left, const Klink::JMath::Vector2& right, const Klink::Graphics::Colours::Colour color)
{
	DrawScreenLine(tip, left, color);
	DrawScreenLine(tip, right, color);
	DrawScreenLine(left, right, color);
}

void SimpleDraw::DrawDebugGrid(int lineCount, const Klink::Graphics::Colours::Colour color)
{
	// X Axis
	DrawLine({ -100.0f, 0.0f, 0.0f }, { 100.0f, 0.0f, 0.0f }, Colours::Red);
	// Y Axis
	DrawLine({ 0.0f, -100.0f, 0.0f }, { 0.0f, 100.0f, 0.0f }, Colours::Green);
	// Z Axis
	DrawLine({ 0.0f, 0.0f, -100.0f }, { 0.0f, 0.0f, 100.0f }, Colours::Blue);

	for (int lineNumber = 1; lineNumber < lineCount; ++lineNumber)
	{
		// Z-Axis lines
		DrawLine({ (float)(lineNumber * 1.0f), 0.0f, -100.0f }, { (float)(lineNumber * 1.0f), 0.0f, 100.0f }, color);
		DrawLine({ (float)(-lineNumber * 1.0f), 0.0f, -100.0f }, { (float)(-lineNumber * 1.0f), 0.0f, 100.0f }, color);

		// X-Axis lines
		DrawLine({ -100.0f, 0.0f, (float)(lineNumber * 1.0f) }, { 100.0f, 0.0f, (float)(lineNumber * 1.0f) }, color);
		DrawLine({ -100.0f, 0.0f, (float)(-lineNumber * 1.0f) }, { 100.0f, 0.0f, (float)(-lineNumber * 1.0f) }, color);
	}
}

void SimpleDraw::DrawScreenDebugGrid(int lineSpacing, const Klink::Graphics::Colours::Colour color)
{
	int height = GraphicsSystem::Get()->GetBackBufferHeight();
	int width = GraphicsSystem::Get()->GetBackBufferWidth();

	int halfHeight = height / 2;
	int halfWidth = width / 2;

	if (lineSpacing > 0)
	{
		// X Axis
		DrawScreenLine({ 0.f, (float)halfHeight }, { (float)width, (float)halfHeight }, Colours::Red);
		// Y Axis
		DrawScreenLine({ (float)halfWidth, 0.f }, { (float)halfWidth, (float)height }, Colours::Green);

		float linePosition;
		int xLineCount = height / (lineSpacing*2);
		for (int xIndex = 1; xIndex < xLineCount+1; ++xIndex)
		{
			// X Axis lines
			linePosition = halfHeight + (xIndex * lineSpacing);
			DrawScreenLine({ 0.0f, linePosition }, { (float)width, linePosition }, color);
			linePosition = halfHeight - (xIndex * lineSpacing);
			DrawScreenLine({ 0.0f, linePosition }, { (float)width, linePosition }, color);
		}

		int yLineCount = width / (lineSpacing*2);
		for (int yIndex = 1; yIndex < yLineCount+1; ++yIndex)
		{
			// Y Axis lines
			linePosition = halfWidth + (yIndex * lineSpacing);
			DrawScreenLine({linePosition, 0.0f}, {linePosition, (float)height}, color);
			linePosition = halfWidth - (yIndex * lineSpacing);
			DrawScreenLine({linePosition, 0.0f}, {linePosition, (float)height}, color);
		}

		lineSpacing = 0;
	}
}

void SimpleDraw::DrawLine(const Vector3& v0, const Vector3& v1, const Colours::Colour color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render3D(camera);
	sInstance->Render2D(camera);
}