#include "Precompiled.h"
#include "VertexShader.h"

#include "GraphicsSystem.h"
#include "D3DUtilities.h"
#include "VertexTypes.h"


using namespace Klink::Graphics;

namespace
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector< D3D11_INPUT_ELEMENT_DESC> desc;
		if (vertexFormat & VE_Position)
		{
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_Normal)
		{
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_Tangent)
		{
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_Color)
		{
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_Texcoord)
		{
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_BlendIndex)
		{
			desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (vertexFormat & VE_BlendWeights)
		{
			desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		return desc;
	}
}

void VertexShader::Initialize(std::string shaderName, uint32_t vertexFormat)
{
	//Describes what a Vertex Struct is to the GPU
	auto elemDescs = GetVertexLayout(vertexFormat);

	HRESULT hr;

	//Compile our vertex shader code
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::wstring name = ConvertString(shaderName);
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION;
	hr = D3DCompileFromFile(name.c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
	ASSERT(SUCCEEDED(hr), "[VertexShader] Failed to compile vertex shader! Error: %s", (const char*)errorBlob->GetBufferPointer());


	hr = GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);
	ASSERT(SUCCEEDED(hr), "[VertexShader] Failed to create vertex shader!");

	hr = GetDevice()->CreateInputLayout(elemDescs.data(), (UINT)elemDescs.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &mInputLayout);

	ASSERT(SUCCEEDED(hr), "[VertexShader] Failed to create input layout!");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void VertexShader::Bind()
{
	GetDeviceContext()->IASetInputLayout(mInputLayout);
	GetDeviceContext()->VSSetShader(mVertexShader, nullptr, 0);
}

void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
}

VertexShader::~VertexShader()
{
	ASSERT(!mVertexShader || !mInputLayout, "[VertexShader] VertexShader was not terminated correctly!");
}

std::wstring VertexShader::ConvertString(std::string & s)
{
	int length;
	int stringLength = (int)s.length() + 1;

	length = MultiByteToWideChar(CP_ACP, 0, s.c_str(), stringLength, 0, 0);
	wchar_t* buffer = new wchar_t[length];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), stringLength, buffer, length);
	std::wstring r(buffer);

	delete[] buffer;
	return r;
}