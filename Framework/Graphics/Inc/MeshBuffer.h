#ifndef INCLUDED_KLINK_GRAPHICS_MESHBUFFER_H
#define INCLUDED_KLINK_GRAPHICS_MESHBUFFER_H

#include "Mesh.h"

namespace Klink::Graphics
{

class MeshBuffer
{
public:

	enum Topology
	{
		Points,
		Lines,
		Triangles
	};

	template<class MeshType>
	void Initialize(const MeshType& mesh, bool dynamic = false);
	void Initialize(size_t numVertices, int vertexSize, const void* data, size_t numIndices, const uint32_t* indexData, bool isDynamic = false);
	void Initialize(int dataSize, int vertexSize, const void* data, bool dynamic = false);

	void Bind();
	void Render();
	void Update(size_t vertexSize, const void* data);
	void SetTopology(Topology topology);

	void Terminate();
	//~MeshBuffer();

private:

	void CreateVertexBuffer(size_t dataSize, int vertexSize, const void* data, bool dynamic);
	void CreateIndexBuffer(size_t numIndices, const uint32_t* indexData);

	ID3D11Buffer* mVertexBuffer{ nullptr };
	ID3D11Buffer* mIndexBuffer{ nullptr };
	size_t mVertexCount{ 0 };
	size_t mVertexSize{ 0 };
	size_t mIndexCount{ 0 };
	D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
	bool mIsDynamic{ false };
};

template<class MeshType>
inline void Klink::Graphics::MeshBuffer::Initialize(const MeshType & mesh, bool dynamic)
{
	Initialize(mesh.mVertices.size(), sizeof(MeshType::VertexType), mesh.mVertices.data(), mesh.mIndices.size(), mesh.mIndices.data(), dynamic);
}

} // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_MESHBUFFER_H