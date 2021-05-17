#ifndef INCLUDED_KLINK_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_KLINK_GRAPHICS_VERTEXSHADER_H

namespace Klink::Graphics
{

class VertexShader
{
public:

	void Initialize(std::string shaderName, uint32_t vertexFormat);

	void Bind();
	
	void Terminate();
	~VertexShader();

private:

	std::wstring ConvertString(std::string& s);

	ID3D11VertexShader* mVertexShader{ nullptr };
	ID3D11InputLayout* mInputLayout{ nullptr };
};

} // namespace Klink::Graphics


#endif // INCLUDED_VISHV_GRAPHICS_VERTEX_SHADER_H
