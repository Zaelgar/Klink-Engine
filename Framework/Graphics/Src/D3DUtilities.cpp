#include "Precompiled.h"
#include "D3DUtilities.h"
#include "GraphicsSystem.h"
#include "Texture.h"

ID3D11Device* Klink::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->GetDevice();
}

ID3D11DeviceContext* Klink::Graphics::GetDeviceContext()
{
	return GraphicsSystem::Get()->GetDeviceContext();
}

ID3D11ShaderResourceView* Klink::Graphics::GetShaderResourceView(const Texture& texture)
{
	return texture.mShaderResourceView;
}