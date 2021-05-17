#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"

using namespace Klink::Graphics;

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Terminate must be called before Destruction");
}

bool Texture::Initialize(const std::filesystem::path& fileName)
{
	// std::wstring name = ConvertString(fileName);

	ID3D11DeviceContext* deviceContext = GetDeviceContext();
	ID3D11Device* device = GetDevice();
	
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, deviceContext, fileName.c_str(), nullptr, &mShaderResourceView);

	ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture %ls", fileName.c_str());
	return true;	
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	GetDeviceContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	GetDeviceContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}