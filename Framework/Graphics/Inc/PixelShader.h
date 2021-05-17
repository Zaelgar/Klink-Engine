#ifndef INCLUDED_KLINK_GRAPHICS_PIXELSHADER_H
#define INCLUDED_KLINK_GRAPHICS_PIXELSHADER_H

namespace Klink::Graphics
{

class PixelShader
{
public:

	PixelShader() = default;
	~PixelShader();

	void Initialize(std::string shaderName);
	void Bind();

	void Terminate();

	// Accessors
	inline ID3D11PixelShader* GetPixelShader() const { return mPixelShader; }

private:

	std::wstring ConvertString(std::string& string);

	ID3D11PixelShader* mPixelShader = nullptr;
};

} // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_PIXELSHADER_H