#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"

using namespace Klink::Graphics;
using namespace Klink::JMath;

void ConstantBuffer::Initialize(uint32_t size, const void* initData)
{
	D3D11_BUFFER_DESC cBufferDesc = {};
	cBufferDesc.ByteWidth = size;
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData{};
	subresourceData.pSysMem = initData;

	GetDevice()->CreateBuffer(&cBufferDesc, initData ? &subresourceData : nullptr, &mConstantBuffer);
}

void ConstantBuffer::Set(const void* matrix)
{
	GetDeviceContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, matrix, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	GetDeviceContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	GetDeviceContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindGS(uint32_t slot) const
{
	GetDeviceContext()->GSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindCS(uint32_t slot) const
{
	GetDeviceContext()->CSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(!mConstantBuffer, "[ConstantBuffer] Constant buffer not terminated correctly!");
}