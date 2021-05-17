#ifndef INCLUDED_KLINK_GRAPHICS_SAMPLER_H
#define INCLUDED_KLINK_GRAPHICS_SAMPLER_H

namespace Klink::Graphics
{

class Sampler
{
public:

	enum class Filter { Point, Linear, Anisotropic };
	enum class AddressMode { Border, Clamp, Mirror, Wrap, };

	Sampler() = default;
	~Sampler();

	void Initialize(Filter, AddressMode);
	void Terminate();

	void BindPS(uint32_t slot = 0) const;

private:

	ID3D11SamplerState* mSampler{ nullptr };
};

};	// namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_SAMPLER_H