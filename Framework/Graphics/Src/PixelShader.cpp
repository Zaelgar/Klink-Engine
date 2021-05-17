#include "Precompiled.h"
#include "PixelShader.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"

using namespace Klink::Graphics;

void PixelShader::Initialize(std::string shaderName)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::wstring name = ConvertString(shaderName);
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT	hr = D3DCompileFromFile(name.c_str(),
		nullptr,
		nullptr,
		"PS",
		"ps_5_0",
		compileFlags, 0,
		&shaderBlob,
		&errorBlob);

	ASSERT(SUCCEEDED(hr), "[PixelShader] Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	hr = GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	ASSERT(SUCCEEDED(hr), "[PixelShader] Failed to Create pixel shader!");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void PixelShader::Bind()
{
	GetDeviceContext()->PSSetShader(mPixelShader, nullptr, 0);
}

void Klink::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

std::wstring Klink::Graphics::PixelShader::ConvertString(std::string & string)
{
	int length;
	int slength = (int)string.length() + 1;
	length = MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, 0, 0);
	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, buffer, length);
	std::wstring r(buffer);
	delete[] buffer;
	return r;
}

PixelShader::~PixelShader()
{
	ASSERT(!mPixelShader, "[PixelShader] Terminate not called correctly!");
}