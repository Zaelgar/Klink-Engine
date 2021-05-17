#ifndef INCLUDED_KLINK_GRAPHICS_GRAPHICSSYSTEM_H
#define INCLUDED_KLINK_GRAPHICS_GRAPHICSSYSTEM_H

#include "Colors.h"

namespace Klink::Graphics
{

class  GraphicsSystem
{
public:

	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

	GraphicsSystem() = default;
	~GraphicsSystem();

	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;

	void BeginRender();
	void BeginRender(const FLOAT clearColour[]);
	void BeginRender(const Graphics::Colours::Colour color);
	void EndRender();

	void SetClearColour(Colours::Colour colour) { mClearColour[0] = colour.r; mClearColour[1] = colour.g; mClearColour[2] = colour.b; mClearColour[3] = colour.a; }
	void SetVSync(bool vSync) { mVSync = vSync; }

	void ToggleFullscreen();
	void ResizeWindow(uint32_t width, uint32_t height);

	void ResetRenderTarget();
	void ResetViewport();

private:

	//friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	friend ID3D11Device* GetDevice();
	friend ID3D11DeviceContext* GetDeviceContext();

	FLOAT mClearColour[4]{0.0f, 0.0f, 0.0f, 1.0f};
	int mVSync = 1;

	ID3D11Device* GetDevice() { return mD3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return mD3DDeviceContext; }

	ID3D11Device* mD3DDevice{ nullptr };
	ID3D11DeviceContext* mD3DDeviceContext{ nullptr };

	IDXGISwapChain* mSwapChain{ nullptr };
	ID3D11RenderTargetView* mRenderTargetView{ nullptr };
	ID3D11Texture2D* mDepthStencilBuffer{ nullptr };
	ID3D11DepthStencilView* mDepthStencilView{ nullptr };
	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
	D3D11_VIEWPORT mViewport;
};

} // namespace Klink::Graphics

#endif // #ifndef INCLUDED_GRAPHICS_GRAPHICSSYSTEM