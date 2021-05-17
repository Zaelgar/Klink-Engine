#include "Precompiled.h"
#include "MeshBuffer.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"

using namespace Klink::Graphics;

void MeshBuffer::Initialize(size_t numVertices, int vertexSize, const void* data, size_t numIndices, const uint32_t* indexData, bool isDynamic)
{
	CreateVertexBuffer(numVertices, vertexSize, data, isDynamic);
	CreateIndexBuffer(numIndices, indexData);
}

void MeshBuffer::Initialize(int dataSize, int vertexSize, const void* data, bool isDynamic)
{
	CreateVertexBuffer(dataSize, vertexSize, data, isDynamic);
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void MeshBuffer::CreateVertexBuffer(size_t dataSize, int vertexSize, const void * data, bool isDynamic)
{
	mVertexCount = dataSize;
	mVertexSize = vertexSize;

	D3D11_BUFFER_DESC vertexBufferDesc{};

	vertexBufferDesc.ByteWidth = (UINT)(mVertexCount * mVertexSize);
	vertexBufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = data;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = GetDevice()->CreateBuffer(&vertexBufferDesc, data ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "[MeshBuffer] Failed to create vertex buffer!");
}

void MeshBuffer::CreateIndexBuffer(size_t numIndices, const uint32_t* indexData)
{
	mIndexCount = numIndices;

	D3D11_BUFFER_DESC indexBufferDesc{};

	indexBufferDesc.ByteWidth = (UINT)(mIndexCount * sizeof(uint32_t));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Creat init Data
	D3D11_SUBRESOURCE_DATA indexBufferData{};
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	indexBufferData.pSysMem = indexData;

	HRESULT hr = GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "[MeshBuffer] Failed to create index buffer!");
}

void MeshBuffer::Bind()
{
	UINT stride = (UINT)mVertexSize;
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
}

void MeshBuffer::Update(size_t count, const void* data)
{
	mVertexCount = count;
	auto context = GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, mVertexCount * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::Render()
{
	ID3D11DeviceContext* deviceContext = GetDeviceContext();

	UINT stride = (UINT)mVertexSize;
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// Set primitive topology
	deviceContext->IASetPrimitiveTopology(mTopology);

	if (mIndexBuffer != nullptr) // Draw indexed
	{
		//Set Index Buffer
		deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Draw Indexed Mesh
		deviceContext->DrawIndexed((UINT)mIndexCount, 0, 0);
	}
	else // Draw unindexed
	{
		deviceContext->Draw((UINT)mVertexCount, 0);
	}
}

void MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

// TODO - Destructor fix for vectors (std::vector<MeshBuffer> deleted function)
/*
MeshBuffer::~MeshBuffer()
{
	ASSERT((!mVertexBuffer && !mIndexBuffer), "[MeshBuffer] MeshBuffer was not terminated correctly!");
}
*/