#ifndef INCLUDED_KLINK_GRAPHICS_D3DUTILITIES_H
#define INCLUDED_KLINK_GRAPHICS_D3DUTILITIES_H

namespace Klink::Graphics
{

class Texture;

ID3D11Device* GetDevice();
ID3D11DeviceContext* GetDeviceContext();

ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);

};	// namespace Klink::Graphics::D3DUtilities

#endif // INCLUDED_KLINK_GRAPHICS_D3DUTILITIES_H