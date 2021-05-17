#ifndef INCLUDED_KLINK_GRAPHICS_MESH_H 
#define INCLUDED_KLINK_GRAPHICS_MESH_H

#include "VertexTypes.h"

namespace Klink::Graphics
{
	template<class VertexT>
	struct BaseMesh
	{
		using VertexType = VertexT;
		std::vector<VertexT> mVertices;
		std::vector<uint32_t> mIndices;
	};

	using MeshPC = BaseMesh<VertexPC>;
	using MeshPNX = BaseMesh<VertexPNX>;
	using MeshPX = BaseMesh<VertexPX>;
	using MeshPN = BaseMesh<VertexPN>;
	// Combined, standard vertex and mesh
	using Mesh = BaseMesh<Vertex>;

	using BoneMesh = BaseMesh<BoneVertex>;

} // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_MESH_H