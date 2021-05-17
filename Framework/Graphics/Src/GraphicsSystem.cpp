#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace Klink::Core;
using namespace Klink::Graphics;

namespace
{
	std::unique_ptr<GraphicsSystem> sGraphicsSystem = nullptr;
	WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
			const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
			sGraphicsSystem->ResizeWindow(width, height);
			break;
		}
		default:
			break;
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	// use unique pointers instead of normal pointers. Newer c++
	ASSERT(sGraphicsSystem == nullptr, "[Graphics::GraphicsSystem] System already initiaized!");
	sGraphicsSystem = std::make_unique<GraphicsSystem>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		// have to call reset instead of delete, (smart pointer functionality)
		sGraphicsSystem.reset();
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[Graphics::GraphicsSystem] No System Registered.");

	return sGraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
	// restore

	ASSERT(mD3DDevice == nullptr, "[Graphics::GraphicsSystem] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	// getting window dimensions
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);


	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 	// buffer is a 3d array, so the format RGB and alpha 8  bits for each equal to 32 bits. unorm means its ---- and normalized
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // what we are using this buffer for
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;

	const D3D_FEATURE_LEVEL kFeatureLevel = { D3D_FEATURE_LEVEL_11_0 };


	//HResult is just a typedef for a long
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&kFeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		nullptr,
		&mD3DDeviceContext
	);

	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create device or swapchain. ");

	//cacge swap chain description
	mSwapChain->GetDesc(&mSwapChainDesc);

	ResizeWindow(GetBackBufferWidth(), GetBackBufferHeight());

	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}

void Klink::Graphics::GraphicsSystem::Terminate()
{
	sWindowMessageHandler.Unhook();
	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mD3DDeviceContext);
	SafeRelease(mD3DDevice);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
}

uint32_t Klink::Graphics::GraphicsSystem::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t Klink::Graphics::GraphicsSystem::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}

void Klink::Graphics::GraphicsSystem::BeginRender()
{
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetView, mClearColour);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Klink::Graphics::GraphicsSystem::BeginRender(const FLOAT clearColour[])
{
	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColour);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Klink::Graphics::GraphicsSystem::BeginRender(const Graphics::Colours::Colour color)
{
	const FLOAT clear[4] = {color.r, color.g, color.b, color.a};

	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetView, clear);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Klink::Graphics::GraphicsSystem::EndRender()
{
	mSwapChain->Present(mVSync, 0);
}

void Klink::Graphics::GraphicsSystem::ToggleFullscreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void Klink::Graphics::GraphicsSystem::ResizeWindow(uint32_t width, uint32_t height)
{
	mD3DDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to resize swap chain buffer.");

		// Cache swap chain desc
		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	// Recreate the back buffer for render target
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to access swap chain buffer.");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create render target view.");

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = GetBackBufferWidth();
	descDepth.Height = GetBackBufferHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	// Create and check
	hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create new Depth Stencil Buffer");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = descDepth.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create and check
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create depth stencil view. ");

	//Set the render target view and the depth stencil view
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//setup viewport
	mViewport.Width = static_cast<float>(GetBackBufferWidth());
	mViewport.Height = static_cast<float>(GetBackBufferHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mD3DDeviceContext->RSSetViewports(1, &mViewport);
}

void Klink::Graphics::GraphicsSystem::ResetRenderTarget()
{
	ASSERT(mD3DDeviceContext != nullptr, "[GraphicsSystem] System is not initialized context does not exist");
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void Klink::Graphics::GraphicsSystem::ResetViewport()
{
	mD3DDeviceContext->RSSetViewports(1, &mViewport);
}