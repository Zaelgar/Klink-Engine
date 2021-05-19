#include "Precompiled.h"
#include "MeshBuilder.h"

#include "GraphicsSystem.h"


using namespace Klink::Graphics;
using namespace Klink::JMath;

BaseMesh<VertexPC> MeshBuilder::CreatePCCube(float dimension, Klink::Graphics::Colours::Colour colour)
{
	dimension /= -2.0f;

	BaseMesh<VertexPC> mesh{};

	// Push back mesh.mVertices
	{
		mesh.mVertices.push_back({ {-dimension, dimension, dimension},  {colour} });	// 0
		mesh.mVertices.push_back({ {dimension, dimension, dimension},   {colour} });	// 1
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, {colour} });	// 2
		mesh.mVertices.push_back({ {dimension, dimension, -dimension},  {colour} });	// 3
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, {colour} });	// 4
		mesh.mVertices.push_back({ {dimension, -dimension, dimension},  {colour} });	// 5
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension},{colour} });	// 6
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, {colour} });	// 7
	}

	// Push back mesh.mIndices
	{
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(2);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(2);

		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(7);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(7);

		mesh.mIndices.push_back(2);
		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(2);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(6);
		mesh.mIndices.push_back(4);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(4);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(5);

		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(4);

		mesh.mIndices.push_back(6);
		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(4);
	}

	return mesh;
}

BaseMesh<VertexPX> MeshBuilder::CreateUVCube(float dimension)
{
	dimension /= 2.0f;

	BaseMesh<VertexPX> mesh{};

	// Push back mesh.mVertices of cube
	{
		mesh.mVertices.push_back({ {-dimension, dimension, dimension},   {0.0f, 0.0f} });	// 0 top	0
		mesh.mVertices.push_back({ {-dimension, dimension, dimension},   {0.0f, 0.0f} });	// 0 left	1
		mesh.mVertices.push_back({ {-dimension, dimension, dimension},   {1.0f, 0.0f} });	// 0 back	2

		mesh.mVertices.push_back({ {dimension, dimension, dimension},    {1.0f, 0.0f} });	// 1 top	3
		mesh.mVertices.push_back({ {dimension, dimension, dimension},    {1.0f, 0.0f} });	// 1 right	4
		mesh.mVertices.push_back({ {dimension, dimension, dimension},    {0.0f, 0.0f} });	// 1 back	5

		mesh.mVertices.push_back({ {-dimension, dimension, -dimension},  {0.0f, 1.0f} });	// 2 top	6
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension},  {1.0f, 0.0f} });	// 2 left	7
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension},  {0.0f, 0.0f} });	// 2 front	8

		mesh.mVertices.push_back({ {dimension, dimension, -dimension},   {1.0f, 1.0f} });	// 3 top	9
		mesh.mVertices.push_back({ {dimension, dimension, -dimension},   {0.0f, 0.0f} });	// 3 right	10
		mesh.mVertices.push_back({ {dimension, dimension, -dimension},   {1.0f, 0.0f} });	// 3 front	11

		mesh.mVertices.push_back({ {-dimension, -dimension, dimension},  {0.0f, 1.0f} });	// 4 bottom	12
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension},  {0.0f, 1.0f} });	// 4 left	13
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension},  {1.0f, 1.0f} });	// 4 back	14

		mesh.mVertices.push_back({ {dimension, -dimension, dimension},   {1.0f, 1.0f} });	// 5 bottom	15
		mesh.mVertices.push_back({ {dimension, -dimension, dimension},   {1.0f, 1.0f} });	// 5 right	16
		mesh.mVertices.push_back({ {dimension, -dimension, dimension},   {0.0f, 1.0f} });	// 5 back	17

		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {0.0f, 0.0f} });	// 6 bottom	18
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {1.0f, 1.0f} });	// 6 left	19
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {0.0f, 1.0f} });	// 6 front	20

		mesh.mVertices.push_back({ {dimension, -dimension, -dimension},  {1.0f, 0.0f} });	// 7 bottom	21
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension},  {0.0f, 1.0f} });	// 7 right	22
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension},  {1.0f, 1.0f} });	// 7 front	23
	}
	// Push back mesh.mIndices of cube
	{
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(9);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(10);
		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(16);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(8);
		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(23);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(19);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(2);
		mesh.mIndices.push_back(14);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(14);
		mesh.mIndices.push_back(17);

		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(15);
		mesh.mIndices.push_back(12);

		mesh.mIndices.push_back(18);
		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(12);
	}

	return mesh;
}

BaseMesh<VertexPX> MeshBuilder::CreateUVCylinder(float diameter, float length, int xVertCount, int yVertCount)
{
	// Defaults and minimums
	if (length == 0)
	{
		length = 1.0f;
	}
	if (diameter == 0)
	{
		diameter = 1.0f;
	}
	if (xVertCount < 3)
	{
		xVertCount = 3;
	}
	if (yVertCount < 1)
	{
		yVertCount = 1;
	}

	BaseMesh<VertexPX> mesh{};

	// Precalculated variables
	float radius = diameter / 2.0f;
	float radiusRing = diameter / 2.0f;
	float halfLength = length / 2.0f;

	float thetaIncrement = (2.0f * Pi) / (xVertCount);
	float lengthIncrement = length / yVertCount;
	float uIncrement = 1.0f / xVertCount;
	float vIncrement = 1.0f / yVertCount;

	// float values for each vertex push_back()
	float vX;
	float vY;
	float vZ;
	float u;
	float v;

	int indexOffset = 0;


	// Creating the cylinder top
	// Center Vertex
	int stride = xVertCount;
	mesh.mVertices.push_back(VertexPX
	(
		{ {0.0f, halfLength, 0.0f}, {0.5f, 0.5f} }
	));

	// Circle on top
	for (int xVert = 0; xVert < xVertCount; ++xVert)
	{
		vX = radius * cos(-thetaIncrement * xVert);
		vZ = radius * sin(-thetaIncrement * xVert);
		u = (vX / radius + 1) * 0.5f;
		v = (vZ / radius + 1) * 0.5f;

		mesh.mVertices.push_back(VertexPX
		(
			{ {vX, halfLength, vZ}, {u, v} }
		));
	}
	// Indices with each triangle using center point
	for (int index = 0; index < xVertCount; ++index)
	{
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(index + 1);
		if (index != 0 && (index + 1) % stride == 0)	// At the edge, wrap around
		{
			mesh.mIndices.push_back(1);
			continue;
		}
		mesh.mIndices.push_back(index + 2);
	}
	int vertexOffset = (int)mesh.mVertices.size();



	// Create cylinder center
	for (int yVert = 0; yVert < yVertCount + 1; ++yVert)
	{
		vY = halfLength - (lengthIncrement * yVert);
		v = vIncrement * yVert;

		for (int xVert = 0; xVert < xVertCount; ++xVert)
		{
			vX = radius * cos(thetaIncrement * xVert);
			vZ = radius * sin(thetaIncrement * xVert);
			u = uIncrement * xVert;

			mesh.mVertices.push_back(VertexPX
			(
				{ {vX, vY, vZ}, {u, v} }
			));
		}

		// Remember to create a vertex at the original circle position with correct UV coords
		vX = (float)(radius * cos(0));
		vZ = (float)(radius * sin(0));
		u = 1.0f;
		mesh.mVertices.push_back(VertexPX
		(
			{ {vX, vY, vZ}, {u, v} }
		));
	}
	stride = xVertCount + 1;


	for (int index = 0; index < ((xVertCount + 1) * yVertCount);)
	{
		//tri 1
		mesh.mIndices.push_back(index + vertexOffset);
		mesh.mIndices.push_back(index + 1 + vertexOffset);
		mesh.mIndices.push_back(index + stride + vertexOffset);

		//tri 2
		mesh.mIndices.push_back(index + 1 + vertexOffset);
		mesh.mIndices.push_back(index + 1 + stride + vertexOffset);
		mesh.mIndices.push_back(index + stride + vertexOffset);

		index++;
		if (index != 0 && (index + 1) % (xVertCount + 1) == 0) // Last vertex in row, triangles already assembled, skip to next index
		{
			index++;
		}
	}
	vertexOffset = (int)mesh.mVertices.size();


	// Creating the cylinder bottom
	// Center Vertex
	stride = xVertCount;
	mesh.mVertices.push_back(VertexPX
	(
		{ {0.0f, -halfLength, 0.0f}, {0.5f, 0.5f} }
	));

	// Circle on bottom
	for (int xVert = 0; xVert < xVertCount; ++xVert)
	{
		vX = radius * cos(thetaIncrement * xVert);
		vZ = radius * sin(thetaIncrement * xVert);
		u = (vX / radius + 1) * 0.5f;
		v = (vZ / radius + 1) * 0.5f;

		mesh.mVertices.push_back(VertexPX
		(
			{ {-vX, -halfLength, vZ}, {u, v} }
		));
	}
	// Indices with each triangle using center point
	for (int index = 0; index < xVertCount; ++index)
	{
		mesh.mIndices.push_back(vertexOffset);
		mesh.mIndices.push_back(index + 1 + vertexOffset);
		if (index != 0 && (index + 1) % stride == 0)	// At the edge, wrap around
		{
			mesh.mIndices.push_back(vertexOffset + 1);
			continue;
		}
		mesh.mIndices.push_back(index + 2 + vertexOffset);
	}

	return mesh;
}

BaseMesh<VertexPX> MeshBuilder::CreateUVSphere(float diameter, int xVertCount, int yVertCount)
{
	// Defaults and minimums
	if (diameter < 0)
	{
		diameter = 1.0f;
	}
	if (xVertCount < 3)
	{
		xVertCount = 3;
	}
	if (yVertCount < 2)
	{
		yVertCount = 2;
	}

	BaseMesh<VertexPX> mesh{};

	// Precalculated variables
	float radius = diameter / 2.0f;

	float thetaIncrement = Pi / (yVertCount);	// +2 including poles
	float phiIncrement = (2.0f * Pi) / (xVertCount);
	float lengthIncrement = radius / yVertCount;
	float uIncrement = 1.0f / xVertCount;
	float vIncrement = 1.0f / yVertCount;

	// float values for each vertex push_back()
	float vX;
	float vY;
	float vZ;
	float u;
	float v;

	int stride = xVertCount + 1;

	VertexPX savedVert{};

	// Create cylinder center
	for (int yVert = 0; yVert <= yVertCount; ++yVert)
	{
		v = vIncrement * yVert;

		for (int xVert = 0; xVert < xVertCount; ++xVert)
		{
			vX = radius * sin(thetaIncrement * yVert) * cos(phiIncrement * xVert);
			vZ = radius * sin(thetaIncrement * yVert) * sin(phiIncrement * xVert);
			vY = radius * cos(thetaIncrement * yVert);

			u = uIncrement * xVert;

			Vector3 normal{ vX, vY, vZ };
			normal.Normalize();

			Vector3 tangent{ -vZ, 0.0f, vX };
			tangent.Normalize();

			//TODO radius multiply in here VVV
			if (xVert == 0)	// saving the vertex so we can duplicate it with correct UV coords at the end of the circle
			{
				savedVert = { {vX, vY, vZ}, {1.0f, v} };
			}

			mesh.mVertices.push_back(VertexPX
			(
				{ {vX, vY, vZ} , {u, v} }
			));
		}

		mesh.mVertices.push_back(savedVert);
	}

	for (int index = 0; index < ((xVertCount + 1) * yVertCount);)
	{
		//tri 1
		mesh.mIndices.push_back(index);
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + stride);

		//tri 2
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + 1 + stride);
		mesh.mIndices.push_back(index + stride);

		index++;
		if (index != 0 && (index + 1) % (xVertCount + 1) == 0) // Last vertex in row, triangles already assembled, skip to next index
		{
			index++;
		}
	}

	return mesh;
}

BaseMesh<VertexPNX> MeshBuilder::CreateUVNSphere(float diameter, int xVertCount, int yVertCount)
{
	// Defaults and minimums
	if (diameter < 0)
	{
		diameter = 1.0f;
	}
	if (xVertCount < 3)
	{
		xVertCount = 3;
	}
	if (yVertCount < 2)
	{
		yVertCount = 2;
	}

	BaseMesh<VertexPNX> mesh{};

	// Precalculated variables
	float radius = diameter / 2.0f;

	float thetaIncrement = Pi / (yVertCount);	// +2 including poles
	float phiIncrement = (2.0f * Pi) / (xVertCount);
	float lengthIncrement = radius / yVertCount;
	float uIncrement = 1.0f / xVertCount;
	float vIncrement = 1.0f / yVertCount;

	// float values for each vertex push_back()
	float vX;
	float vY;
	float vZ;
	float u;
	float v;

	int stride = xVertCount + 1;

	VertexPNX savedVert{};

	// Create cylinder center
	for (int yVert = 0; yVert <= yVertCount; ++yVert)
	{
		v = vIncrement * yVert;

		for (int xVert = 0; xVert < xVertCount; ++xVert)
		{
			vX = radius * sin(thetaIncrement * yVert) * cos(phiIncrement * xVert);
			vZ = radius * sin(thetaIncrement * yVert) * sin(phiIncrement * xVert);
			vY = radius * cos(thetaIncrement * yVert);

			u = uIncrement * xVert;

			if (xVert == 0)	// saving the vertex so we can duplicate it with correct UV coords at the end of the circle
			{
				savedVert = { {-vX, vY, vZ}, {-vX, vY, vZ}, {1.0f, v} };
			}

			mesh.mVertices.push_back(VertexPNX
			(
				{ {-vX, vY, vZ}, {-vX, vY, vZ}, {u, v} }
			));
		}

		mesh.mVertices.push_back(savedVert);
	}

	for (int index = 0; index < ((xVertCount + 1) * yVertCount);)
	{
		//tri 1
		mesh.mIndices.push_back(index);
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + stride);

		//tri 2
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + 1 + stride);
		mesh.mIndices.push_back(index + stride);

		index++;
		if (index != 0 && (index + 1) % (xVertCount + 1) == 0) // Last vertex in row, triangles already assembled, skip to next index
		{
			index++;
		}
	}

	return mesh;
}

BaseMesh<VertexPN> MeshBuilder::CreatePNSphere(float diameter, int xVertCount, int yVertCount)
{
	// Defaults and minimums
	if (diameter < 0)
	{
		diameter = 1.0f;
	}
	if (xVertCount < 3)
	{
		xVertCount = 3;
	}
	if (yVertCount < 2)
	{
		yVertCount = 2;
	}

	BaseMesh<VertexPN> mesh{};

	// Precalculated variables
	float radius = diameter / 2.0f;

	float thetaIncrement = Pi / (yVertCount);	// +2 including poles
	float phiIncrement = (2.0f * Pi) / (xVertCount);
	float lengthIncrement = radius / yVertCount;
	float uIncrement = 1.0f / xVertCount;
	float vIncrement = 1.0f / yVertCount;

	// float values for each vertex push_back()
	float vX;
	float vY;
	float vZ;
	float u;
	float v;

	int stride = xVertCount + 1;

	VertexPN savedVert{};

	// Create cylinder center
	for (int yVert = 0; yVert <= yVertCount; ++yVert)
	{
		v = vIncrement * yVert;

		for (int xVert = 0; xVert < xVertCount; ++xVert)
		{
			vX = radius * sin(thetaIncrement * yVert) * cos(phiIncrement * xVert);
			vZ = radius * sin(thetaIncrement * yVert) * sin(phiIncrement * xVert);
			vY = radius * cos(thetaIncrement * yVert);

			u = uIncrement * xVert;

			if (xVert == 0)	// saving the vertex so we can duplicate it with correct UV coords at the end of the circle
			{
				Vector3 temp = (-vX, vY, vZ);
				temp.Normalize();
				savedVert = { {-vX, vY, vZ}, temp };
			}

			Vector3 temp = (-vX, vY, vZ);
			temp.Normalize();

			mesh.mVertices.push_back(VertexPN
			(
				{ {-vX, vY, vZ}, temp }
			));
		}

		mesh.mVertices.push_back(savedVert);
	}

	for (int index = 0; index < ((xVertCount + 1) * yVertCount);)
	{
		//tri 1
		mesh.mIndices.push_back(index);
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + stride);

		//tri 2
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + 1 + stride);
		mesh.mIndices.push_back(index + stride);

		index++;
		if (index != 0 && (index + 1) % (xVertCount + 1) == 0) // Last vertex in row, triangles already assembled, skip to next index
		{
			index++;
		}
	}

	return mesh;
}

BaseMesh<VertexPX> Klink::Graphics::MeshBuilder::CreateSkycube(float dimension)
{
	BaseMesh<VertexPX> mesh{};

	dimension /= 2.0f;

	// Push back mesh.mVertices of cube
	{
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, {0.251f, 0.3326f} });		// 0 top	0
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, {0.249f, 0.334f} });		// 0 left	1
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, {0.250f, 0.334f} });		// 0 back	2

		mesh.mVertices.push_back({ {dimension, dimension, dimension}, {0.499f, 0.333f} });		// 1 top	3
		mesh.mVertices.push_back({ {dimension, dimension, dimension}, {0.500f, 0.3337f} });		// 1 right	4
		mesh.mVertices.push_back({ {dimension, dimension, dimension}, {0.499f, 0.333f} });		// 1 back	5

		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, {0.251f, 0.000f} });	// 2 top	6
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, {0.000f, 0.334f} });	// 2 left	7
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, {1.000f, 0.334f} });	// 2 front	8

		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, {0.499f, 0.000f} });		// 3 top	9
		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, {0.751f, 0.334f} });		// 3 right	10
		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, {0.750f, 0.334f} });		// 3 front	11

		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, {0.251f, 0.666f} });	// 4 bottom	12
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, {0.251f, 0.666f} });	// 4 left	13
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, {0.251f, 0.666f} });	// 4 back	14

		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, {0.499f, 0.666f} });		// 5 bottom	15
		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, {0.500f, 0.6654f} });	// 5 right	16
		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, {0.499f, 0.666f} });		// 5 back	17

		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {0.251f, 1.000f} });	// 6 bottom	18
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {0.000f, 0.666f} });	// 6 left	19
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, {1.000f, 0.666f} });	// 6 front	20

		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, {0.499f, 1.000f} });	// 7 bottom	21
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, {0.750f, 0.666f} });	// 7 right	22
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, {0.751f, 0.666f} });	// 7 front	23
	}
	// Push back mesh.mIndices of cube
	{
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(9);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(10);
		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(16);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(8);
		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(23);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(19);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(2);
		mesh.mIndices.push_back(14);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(14);
		mesh.mIndices.push_back(17);

		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(15);
		mesh.mIndices.push_back(12);

		mesh.mIndices.push_back(18);
		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(12);
	}

	return mesh;
}

BaseMesh<Vertex> MeshBuilder::CreateSphere(float diameter, int xVertCount, int yVertCount)
{
	/*
	Notes:
	top of sphere normal will be 0, 1, 0
	since Y doesn't transfer, cross in shader will also be nothing.
	*/


	// Defaults and minimums
	if (diameter < 0)
	{
		diameter = 1.0f;
	}
	if (xVertCount < 3)
	{
		xVertCount = 3;
	}
	if (yVertCount < 2)
	{
		yVertCount = 2;
	}

	Mesh mesh{};

	// Precalculated variables
	float radius = diameter / 2.0f;

	float thetaIncrement = Pi / (yVertCount);	// +2 including poles
	float phiIncrement = (2.0f * Pi) / (xVertCount);
	float lengthIncrement = radius / yVertCount;
	float uIncrement = 1.0f / xVertCount;
	float vIncrement = 1.0f / yVertCount;

	// float values for each vertex push_back()
	float vX;
	float vY;
	float vZ;
	float u;
	float v;

	int stride = xVertCount + 1;

	Vertex savedVert{};

	// Create cylinder center
	for (int yVert = 0; yVert <= yVertCount; ++yVert)
	{
		v = vIncrement * yVert;

		for (int xVert = 0; xVert < xVertCount; ++xVert)
		{
			vX = radius * sin(thetaIncrement * yVert) * cos(phiIncrement * xVert);
			vZ = radius * sin(thetaIncrement * yVert) * sin(phiIncrement * xVert);
			vY = radius * cos(thetaIncrement * yVert);

			u = uIncrement * xVert;

			Vector3 normal{ vX, vY, vZ };
			normal.Normalize();

			Vector3 tangent{ -vZ, 0.0f, vX };
			tangent.Normalize();

			//TODO radius multiply in here VVV
			if (xVert == 0)	// saving the vertex so we can duplicate it with correct UV coords at the end of the circle
			{
				savedVert = { {vX, vY, vZ}, normal, tangent, {1.0f, v} };
			}

			mesh.mVertices.push_back(Vertex
			(
				{ {vX, vY, vZ}, normal, tangent, {u, v} }
			));
		}

		mesh.mVertices.push_back(savedVert);
	}

	for (int index = 0; index < ((xVertCount + 1) * yVertCount);)
	{
		//tri 1
		mesh.mIndices.push_back(index);
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + stride);

		//tri 2
		mesh.mIndices.push_back(index + 1);
		mesh.mIndices.push_back(index + 1 + stride);
		mesh.mIndices.push_back(index + stride);

		index++;
		if (index != 0 && (index + 1) % (xVertCount + 1) == 0) // Last vertex in row, triangles already assembled, skip to next index
		{
			index++;
		}
	}

	return mesh;
}

BaseMesh<Vertex> MeshBuilder::CreateCube(float dimension)
{
	dimension /= 2.0f;

	BaseMesh<Vertex> mesh{};

	Vector3 topNormal{ 0.0f,  1.0f,  0.0f };
	Vector3 botNormal{ 0.0f, -1.0f,  0.0f };
	Vector3 leftNormal{ -1.0f,  0.0f,  0.0f };
	Vector3 rightNormal{ 1.0f,  0.0f,  0.0f };
	Vector3 frontNormal{ 0.0f,  0.0f, -1.0f };
	Vector3 backNormal{ 0.0f,  0.0f,  1.0f };

	// Push back mesh.mVertices of cube
	{
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, topNormal, {}, {0.0f, 0.0f} });		// 0 top	0
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, leftNormal, {}, {0.0f, 0.0f} });	// 0 left	1
		mesh.mVertices.push_back({ {-dimension, dimension, dimension}, backNormal, {}, {1.0f, 0.0f} });	// 0 back	2

		mesh.mVertices.push_back({ {dimension, dimension, dimension}, topNormal, {}, {1.0f, 0.0f} });	// 1 top	3
		mesh.mVertices.push_back({ {dimension, dimension, dimension}, rightNormal, {}, {1.0f, 0.0f} });	// 1 right	4
		mesh.mVertices.push_back({ {dimension, dimension, dimension}, backNormal, {}, {0.0f, 0.0f} });	// 1 back	5

		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, topNormal, {}, {0.0f, 1.0f} });		// 2 top	6
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, leftNormal, {}, {1.0f, 0.0f} });	// 2 left	7
		mesh.mVertices.push_back({ {-dimension, dimension, -dimension}, frontNormal, {}, {0.0f, 0.0f} });	// 2 front	8

		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, topNormal, {}, {1.0f, 1.0f} });	// 3 top	9
		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, rightNormal, {}, {0.0f, 0.0f} });	// 3 right	10
		mesh.mVertices.push_back({ {dimension, dimension, -dimension}, frontNormal, {}, {1.0f, 0.0f} });	// 3 front	11

		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, botNormal, {}, {0.0f, 1.0f} });		// 4 bottom	12
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, leftNormal, {}, {0.0f, 1.0f} });	// 4 left	13
		mesh.mVertices.push_back({ {-dimension, -dimension, dimension}, backNormal, {}, {1.0f, 1.0f} });	// 4 back	14

		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, botNormal, {}, {1.0f, 1.0f} });	// 5 bottom	15
		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, rightNormal, {}, {1.0f, 1.0f} });	// 5 right	16
		mesh.mVertices.push_back({ {dimension, -dimension, dimension}, backNormal, {}, {0.0f, 1.0f} });	// 5 back	17

		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, botNormal, {}, {0.0f, 0.0f} });		// 6 bottom	18
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, leftNormal, {}, {1.0f, 1.0f} });	// 6 left	19
		mesh.mVertices.push_back({ {-dimension, -dimension, -dimension}, frontNormal, {}, {0.0f, 1.0f} });	// 6 front	20

		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, botNormal, {}, {1.0f, 0.0f} });	// 7 bottom	21
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, rightNormal, {}, {0.0f, 1.0f} });	// 7 right	22
		mesh.mVertices.push_back({ {dimension, -dimension, -dimension}, frontNormal, {}, {1.0f, 1.0f} });	// 7 front	23
	}
	// Push back mesh.mIndices of cube
	{
		mesh.mIndices.push_back(0);
		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(3);
		mesh.mIndices.push_back(9);
		mesh.mIndices.push_back(6);

		mesh.mIndices.push_back(10);
		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(4);
		mesh.mIndices.push_back(16);
		mesh.mIndices.push_back(22);

		mesh.mIndices.push_back(8);
		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(11);
		mesh.mIndices.push_back(23);
		mesh.mIndices.push_back(20);

		mesh.mIndices.push_back(1);
		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(7);
		mesh.mIndices.push_back(19);
		mesh.mIndices.push_back(13);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(2);
		mesh.mIndices.push_back(14);

		mesh.mIndices.push_back(5);
		mesh.mIndices.push_back(14);
		mesh.mIndices.push_back(17);

		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(15);
		mesh.mIndices.push_back(12);

		mesh.mIndices.push_back(18);
		mesh.mIndices.push_back(21);
		mesh.mIndices.push_back(12);
	}

	CalculateTangents(mesh);

	return mesh;
}


BaseMesh<Vertex> MeshBuilder::CreateQuad(float xLength, float zLength)
{
	BaseMesh<Vertex> mesh{};

	float xHalf = xLength / 2.0f;
	float zHalf = zLength / 2.0f;

	/*
	JMath::Vector3 position;
	JMath::Vector3 normal;
	JMath::Vector3 tangent;
	/XXX/JMath::Vector3 bitangent;
	JMath::Vector2 UV;
	*/

	Vertex pushBack1 =
	{

	{-xHalf, 0.0f, zHalf},
	{Vector3::YAxis()},
	{},
	{0.0f, 0.0f}

	};
	Vertex pushBack2 =
	{

	{xHalf, 0.0f, zHalf},
	{Vector3::YAxis()},
	{},
	{1.0f, 0.0f}

	};
	Vertex pushBack3 =
	{

	{xHalf, 0.0f, -zHalf},
	{Vector3::YAxis()},
	{},
	{1.0f, 1.0f}

	};
	Vertex pushBack4 =
	{

	{-xHalf, 0.0f, -zHalf},
	{Vector3::YAxis()},
	{},
	{0.0f, 1.0f}

	};

	mesh.mVertices.push_back(pushBack1);
	mesh.mVertices.push_back(pushBack2);
	mesh.mVertices.push_back(pushBack3);
	mesh.mVertices.push_back(pushBack4);

	mesh.mIndices.push_back(0);
	mesh.mIndices.push_back(1);
	mesh.mIndices.push_back(2);
	mesh.mIndices.push_back(0);
	mesh.mIndices.push_back(2);
	mesh.mIndices.push_back(3);

	CalculateTangents(mesh);

	return mesh;
}

void MeshBuilder::CalculateTangentsAndBinormals(BaseMesh<Vertex>& mesh)
{
	int numFaces, currentFace, index;
	Vertex vertex1, vertex2, vertex3;
	Vector3 tangent, binormal, normal;


	// Calculate the number of faces in the model.
	numFaces = mesh.mIndices.size() / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (currentFace = 0; currentFace < numFaces; currentFace++)
	{
		// Get the three vertices for this face from the model.
		vertex1.position.x = mesh.mVertices[mesh.mIndices[index]].position.x;
		vertex1.position.y = mesh.mVertices[mesh.mIndices[index]].position.y;
		vertex1.position.z = mesh.mVertices[mesh.mIndices[index]].position.z;
		vertex1.UV.x = mesh.mVertices[mesh.mIndices[index]].UV.x;
		vertex1.UV.y = mesh.mVertices[mesh.mIndices[index]].UV.y;
		vertex1.normal.x = mesh.mVertices[mesh.mIndices[index]].normal.x;
		vertex1.normal.y = mesh.mVertices[mesh.mIndices[index]].normal.y;
		vertex1.normal.z = mesh.mVertices[mesh.mIndices[index]].normal.z;
		index++;

		vertex2.position.x = mesh.mVertices[mesh.mIndices[index]].position.x;
		vertex2.position.y = mesh.mVertices[mesh.mIndices[index]].position.y;
		vertex2.position.z = mesh.mVertices[mesh.mIndices[index]].position.z;
		vertex2.UV.x = mesh.mVertices[mesh.mIndices[index]].UV.x;
		vertex2.UV.y = mesh.mVertices[mesh.mIndices[index]].UV.y;
		vertex2.normal.x = mesh.mVertices[mesh.mIndices[index]].normal.x;
		vertex2.normal.y = mesh.mVertices[mesh.mIndices[index]].normal.y;
		vertex2.normal.z = mesh.mVertices[mesh.mIndices[index]].normal.z;
		index++;

		vertex3.position.x = mesh.mVertices[mesh.mIndices[index]].position.x;
		vertex3.position.y = mesh.mVertices[mesh.mIndices[index]].position.y;
		vertex3.position.z = mesh.mVertices[mesh.mIndices[index]].position.z;
		vertex3.UV.x = mesh.mVertices[mesh.mIndices[index]].UV.x;
		vertex3.UV.y = mesh.mVertices[mesh.mIndices[index]].UV.y;
		vertex3.normal.x = mesh.mVertices[mesh.mIndices[index]].normal.x;
		vertex3.normal.y = mesh.mVertices[mesh.mIndices[index]].normal.y;
		vertex3.normal.z = mesh.mVertices[mesh.mIndices[index]].normal.z;
		index++;



		// Calculate the tangent and binormal of that face
		{
			float vector1[3], vector2[3];
			float tuVector[2], tvVector[2];
			float den;
			float length;


			// Calculate the two vectors for this face.
			vector1[0] = vertex2.position.x - vertex1.position.x;
			vector1[1] = vertex2.position.y - vertex1.position.y;
			vector1[2] = vertex2.position.z - vertex1.position.z;

			vector2[0] = vertex3.position.x - vertex1.position.x;
			vector2[1] = vertex3.position.y - vertex1.position.y;
			vector2[2] = vertex3.position.z - vertex1.position.z;

			// Calculate the tu and tv texture space vectors.
			tuVector[0] = vertex2.UV.x - vertex1.UV.x;
			tvVector[0] = vertex2.UV.y - vertex1.UV.y;

			tuVector[1] = vertex3.UV.x - vertex1.UV.x;
			tvVector[1] = vertex3.UV.y - vertex1.UV.y;

			// Calculate the denominator of the tangent/binormal equation.
			den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

			// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
			tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
			tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
			tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

			binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
			binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
			binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

			// Calculate the length of this normal.
			length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

			// Normalize the normal and then store it
			tangent.x = tangent.x / length;
			tangent.y = tangent.y / length;
			tangent.z = tangent.z / length;

			// Calculate the length of this normal.
			length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

			// Normalize the normal and then store it
			binormal.x = binormal.x / length;
			binormal.y = binormal.y / length;
			binormal.z = binormal.z / length;
		}



		// Calculate the new normal using the tangent and binormal.
		{
			float length;
			// Calculate the cross product of the tangent and binormal which will give the normal vector.
			normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
			normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
			normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

			// Calculate the length of the normal.
			length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

			// Normalize the normal.
			normal.x = normal.x / length;
			normal.y = normal.y / length;
			normal.z = normal.z / length;
		}


		// Store the normal, tangent, and binormal for this face back in the model structure.
		mesh.mVertices[mesh.mIndices[index - 1]].normal.x = normal.x;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.y = normal.y;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.z = normal.z;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.x = tangent.x;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.y = tangent.y;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.z = tangent.z;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.x = binormal.x;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.y = binormal.y;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.z = binormal.z;

		mesh.mVertices[mesh.mIndices[index - 1]].normal.x = normal.x;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.y = normal.y;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.z = normal.z;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.x = tangent.x;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.y = tangent.y;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.z = tangent.z;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.x = binormal.x;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.y = binormal.y;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.z = binormal.z;

		mesh.mVertices[mesh.mIndices[index - 1]].normal.x = normal.x;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.y = normal.y;
		mesh.mVertices[mesh.mIndices[index - 1]].normal.z = normal.z;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.x = tangent.x;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.y = tangent.y;
		mesh.mVertices[mesh.mIndices[index - 1]].tangent.z = tangent.z;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.x = binormal.x;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.y = binormal.y;
		//mesh.mVertices[mesh.mIndices[index - 1]].bitangent.z = binormal.z;
	}
}

void MeshBuilder::CalculateNormals(BaseMesh<Vertex>& mesh)
{
	for (auto& vert : mesh.mVertices)
	{
		vert.normal = Vector3::Zero();
	}

	Vector3 faceNormal = {};
	for (int index = 0; index < mesh.mIndices.size();)
	{
		faceNormal = Cross(
			mesh.mVertices[mesh.mIndices[index + 1]].position - mesh.mVertices[mesh.mIndices[index]].position,
			mesh.mVertices[mesh.mIndices[index + 2]].position - mesh.mVertices[mesh.mIndices[index]].position
		).Normalized();

		// Since we already have the face and the verts that make that face, we can simply add the normal to its current normals.
		// We will normalize it in another loop below when all possible face normals are accounted for
		mesh.mVertices[mesh.mIndices[index]].normal += faceNormal;
		mesh.mVertices[mesh.mIndices[index + 1]].normal += faceNormal;
		mesh.mVertices[mesh.mIndices[index + 2]].normal += faceNormal;

		index += 3;
	}

	// Each vertex should have a normal that is the average of the normals of the faces that they are apart of.
	for (auto& vert : mesh.mVertices)
	{
		vert.normal.Normalize();
	}
}

void MeshBuilder::CalculateTangents(BaseMesh<Vertex>& mesh)
{
	int numFaces, currentFace;// index;
	Vertex vertex1, vertex2, vertex3;
	Vector3 tangent, binormal, normal;


	// Calculate the number of faces in the model.
	numFaces = mesh.mIndices.size() / 3;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int index = 0; index < mesh.mIndices.size();)
	{
		vertex1 = mesh.mVertices[mesh.mIndices[index++]];
		vertex2 = mesh.mVertices[mesh.mIndices[index++]];
		vertex3 = mesh.mVertices[mesh.mIndices[index++]];

		Vector3 vector1, vector2;
		Vector2 tuVector, tvVector;
		float den;
		float length;


		// Calculate the two vectors for this face.
		vector1 = vertex2.position - vertex1.position;
		vector2 = vertex3.position - vertex1.position;

		// Calculate the tu and tv texture space vectors.
		tuVector.x = vertex2.UV.x - vertex1.UV.x;
		tvVector.x = vertex2.UV.y - vertex1.UV.y;

		tuVector.y = vertex3.UV.x - vertex1.UV.x;
		tvVector.y = vertex3.UV.y - vertex1.UV.y;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector.y * vector1.x - tvVector.x * vector2.x) * den;
		tangent.y = (tvVector.y * vector1.y - tvVector.x * vector2.y) * den;
		tangent.z = (tvVector.y * vector1.z - tvVector.x * vector2.z) * den;

		// Normalize the tangent and then store it
		tangent.Normalize();

		// Store the normal, tangent, and binormal for this face back in the model structure.
		mesh.mVertices[mesh.mIndices[index - 1]].tangent += tangent;
		mesh.mVertices[mesh.mIndices[index - 2]].tangent += tangent;
		mesh.mVertices[mesh.mIndices[index - 3]].tangent += tangent;
	}

	for (auto& vert : mesh.mVertices)
	{
		vert.tangent.Normalize();
	}
}

BaseMesh<VertexPX> MeshBuilder::CreateNDCQuad()
{
	BaseMesh<VertexPX> mesh{};

	mesh.mVertices.emplace_back(VertexPX{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });	// bottom left	// 0
	mesh.mVertices.emplace_back(VertexPX{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } });	// top left		// 1
	mesh.mVertices.emplace_back(VertexPX{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } });	// top right	// 2
	mesh.mVertices.emplace_back(VertexPX{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });	// bottom right	// 3

	mesh.mIndices.emplace_back(0);
	mesh.mIndices.emplace_back(1);
	mesh.mIndices.emplace_back(2);

	mesh.mIndices.emplace_back(0);
	mesh.mIndices.emplace_back(2);
	mesh.mIndices.emplace_back(3);

	return mesh;
}

// Creates a cube around a bone
BaseMesh<BoneVertex> MeshBuilder::CreateBoneCube(Bone& bone, float length, float width)
{
	BaseMesh<BoneVertex> mesh;



	return mesh;
}

BaseMesh<Vertex> Klink::Graphics::MeshBuilder::CreateTerrain(float* heightmap, int xSize, int ySize, float sizeScale, float heightScale)
{
	BaseMesh<Vertex> mesh{};

	// Create array of vertices
	mesh.mVertices.resize(xSize * ySize);
	mesh.mIndices.resize((xSize - 1) * (ySize - 1) * 6);

	float uvXIncrement = 1.0f / xSize;
	float uvYIncrement = 1.0f / ySize;
	int i = 0;
	int t = 0;

	for (int y = 0; y < ySize; ++y)
	{
		for (int x = 0; x < xSize; ++x)
		{
			i = y * ySize + x;

			mesh.mVertices[i].position.x = static_cast<float>((x / (xSize - 1.f) * 2 - 1) * sizeScale);
			mesh.mVertices[i].position.z = static_cast<float>((y / (ySize - 1.f) * 2 - 1) * sizeScale);
			mesh.mVertices[i].position.y = (heightmap[i] * heightScale);

			mesh.mVertices[i].UV.x = uvXIncrement * x;
			mesh.mVertices[i].UV.y = uvYIncrement * y;

			if (x != xSize - 1 && y != ySize - 1)
			{
				mesh.mIndices[t + 0] = i + xSize;
				mesh.mIndices[t + 1] = i + xSize + 1;
				mesh.mIndices[t + 2] = i;

				mesh.mIndices[t + 3] = i + xSize + 1;
				mesh.mIndices[t + 4] = i + 1;
				mesh.mIndices[t + 5] = i;
				t += 6;
			}
		}
	}

	// Calculate Normals and Tangents for rendering
	CalculateNormals(mesh);
	CalculateTangents(mesh);

	return mesh;
}