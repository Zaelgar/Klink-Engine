#include "Precompiled.h"
#include "BlendState.h"

#include "D3DUtilities.h"

using namespace Klink::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::BlendMode mode)
	{
		switch (mode)
		{
		case BlendState::BlendMode::Opaque:
			return D3D11_BLEND_ONE;
		case BlendState::BlendMode::AlphaBlend:
			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::BlendMode::AlphaPremulitplied:
			return D3D11_BLEND_ONE;
		case BlendState::BlendMode::Additive:
			return D3D11_BLEND_SRC_ALPHA;
		default:
			return D3D11_BLEND_ONE;
			break;
		}
	}
	D3D11_BLEND GetDestBlend(BlendState::BlendMode mode)
	{
		switch (mode)
		{
		case BlendState::BlendMode::Opaque:
			return D3D11_BLEND_ZERO;
		case BlendState::BlendMode::AlphaBlend:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::BlendMode::AlphaPremulitplied:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::BlendMode::Additive:
			return D3D11_BLEND_ONE;
		default:
			return D3D11_BLEND_ZERO;
			break;
		}
	}
};

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr,"[BlendState] BlendState not released!");
}

void BlendState::Initialize(BlendMode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE || (destBlend != D3D11_BLEND_ZERO));
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = GetDevice()->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "[BlendState] Failed to create blendstate!");
}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void BlendState::Set()
{
	GetDeviceContext()->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}

void BlendState::Clear()
{
	GetDeviceContext()->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}