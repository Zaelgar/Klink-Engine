/*

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <JMath/Inc/EngineMath.h>

#include <Graphics/Inc/VertexTypes.h>

using namespace Klink::Core;
using namespace Klink::Graphics;
using namespace Klink::JMath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup window, initialise the graphics system
	Klink::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hello Triangle", 1280, 720);

	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);


	// define 3 vertices for rendering triangle
	const Vertex vertices[] =
	{
		{{0.0f, 0.5f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}},
		{{0.5f, 0.0f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}},
		{{-0.5f, 0.0f, 0.0f}, {0.5f, 0.1f, 0.5f, 1.0f}}
	};

	const D3D11_INPUT_ELEMENT_DESC vertexDescs[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};



	// create and initialise passable verte buffer to graphics device
	ID3D11Buffer* vertexBuffer = nullptr;

	auto graphicsDevice = GraphicsSystem::Get()->GetDevice();

	// Setup for vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = std::size(vertices) * sizeof(Vertex);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Setup initialisation data for our vertex buffer
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = graphicsDevice->CreateBuffer(&vertexBufferDesc, &initData, &vertexBuffer);

	ASSERT(SUCCEEDED(hr), "failed to create vertex buffer");

	//Compile vertex and pixel shader
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	hr = D3DCompileFromFile(L"../Assets/Shaders/DoSomething.fx",
		nullptr,
		nullptr,
		"VS",
		"vs_5_0",
		0, 0,
		&shaderBlob,
		&errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile VS shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;

	// Create the vertex shader using data from compile from file
	hr = graphicsDevice->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader);

	ASSERT(SUCCEEDED(hr), "Failed to create VS shader.");

	// Create Input layout??
	hr = graphicsDevice->CreateInputLayout(vertexDescs, std::size(vertexDescs), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	ASSERT(SUCCEEDED(hr), "Failed to create input layout");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);


	///////////////////////////////////////


	shaderBlob = nullptr;
	errorBlob = nullptr;

	hr = D3DCompileFromFile(L"../Assets/Shaders/DoSomething.fx",
		nullptr,
		nullptr,
		"PS",
		"ps_5_0",
		0, 0,
		&shaderBlob,
		&errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile PS shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	ID3D11PixelShader* pixelShader = nullptr;

	// Create the vertex shader using data from compile from file
	hr = graphicsDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);

	ASSERT(SUCCEEDED(hr), "Failed to create PS shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);



	bool done = false;
	while (!done)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}

		done = myWindow.ProcessMessage();


		GraphicsSystem::Get()->BeginRender();


		auto context = GraphicsSystem::Get()->GetDeviceContext();

		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		context->Draw(std::size(vertices), 0);

		GraphicsSystem::Get()->EndRender();
	}

	SafeRelease(vertexBuffer);
	SafeRelease(pixelShader);
	SafeRelease(inputLayout);
	SafeRelease(vertexShader);


	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}
*/