#ifndef INCLUDED_KLINK_GRAPHICS_BLENDSTATE_H
#define INCLUDED_KLINK_GRAPHICS_BLENDSTATE_H

namespace Klink::Graphics
{

class BlendState
{
public:

	enum class BlendMode
	{
		Opaque,
		AlphaBlend,
		AlphaPremulitplied,
		Additive
	};

	BlendState() = default;
	~BlendState();

	void Initialize(BlendMode mode);
	void Terminate();

	void Set();
	void Clear();

private:

	ID3D11BlendState* mBlendState = nullptr;
};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_BLENDSTATE_H