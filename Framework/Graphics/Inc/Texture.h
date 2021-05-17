#ifndef INCLUDED_KLINK_GRAPHICS_TEXTURE_H
#define INCLUDED_KLINK_GRAPHICS_TEXTURE_H

namespace Klink::Graphics
{

class Texture
{
public:

	Texture() = default;
	~Texture();

	bool Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindPS(uint32_t slot = 0) const;
	void BindVS(uint32_t slot = 0) const;

private:

	std::wstring ConvertString(std::string & string);

	friend ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);

	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};

};	// namespace Klink::Graphics

#endif // !INCLUDED_KLINK_GRAPHICS_TEXTURE_H