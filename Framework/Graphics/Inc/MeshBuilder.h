#ifndef INCLUDED_KLINK_GRAPHICS_MESHBUILDER_H
#define INCLUDED_KLINK_GRAPHICS_MESHBUILDER_H

#include "Mesh.h"
#include "Bone.h"

namespace Klink::Graphics
{

class MeshBuilder
{
public:

	MeshBuilder() = default;

	// Returns a mesh of a cube with sides of length "dimension" and of colour "colour".
	static BaseMesh<VertexPC> CreatePCCube(float dimension, Klink::Graphics::Colours::Colour colour);
	static BaseMesh<VertexPX> CreateUVCube(float dimension);
	static BaseMesh<VertexPX> CreateUVCylinder(float diameter, float height, int xVertCount, int yVertCount);
	static BaseMesh<VertexPX> CreateUVSphere(float diameter, int xVertCount, int yVertCount);
	static BaseMesh<VertexPNX> CreateUVNSphere(float diameter, int xVertCount, int yVertCount);
	static BaseMesh<VertexPN> CreatePNSphere(float diameter, int xVertCount, int yVertCount);

	static BaseMesh<VertexPX> CreateSkycube(float dimension);

	static BaseMesh<Vertex> CreateSphere(float diameter, int xVertCount, int yVertCount);
	static BaseMesh<Vertex> CreateCube(float dimension);
	static BaseMesh<Vertex> CreateQuad(float xLength, float yLength);

	static BaseMesh<VertexPX> CreateNDCQuad();

	static BaseMesh<BoneVertex> CreateBoneCube(Bone& bone, float length, float width);

	static BaseMesh<Vertex> CreateTerrain(float* heightmap, int xSize, int ySize, float sizeScale = 1.0f, float heightScale = 1.0f);

	static void CalculateTangents(BaseMesh<Vertex>& mesh);
	static void CalculateTangentsAndBinormals(BaseMesh<Vertex>& mesh);
	static void CalculateNormals(BaseMesh<Vertex>& mesh);

private:
};

}; // namespace Klink::Graphics
#endif // INCLUDED_KLINK_GRAPHICS_MESHBUILDER_H