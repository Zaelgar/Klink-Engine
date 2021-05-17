#ifndef INCLUDED_KLINK_GRAPHICS_CONSTANTBUFFER_H
#define INCLUDED_KLINK_GRAPHICS_CONSTANTBUFFER_H

#include "Common.h"

namespace Klink::Graphics
{

	class ConstantBuffer
	{
	public:

		~ConstantBuffer();

		void Initialize(uint32_t size, const void* initData = nullptr);
		void Terminate();

		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void BindGS(uint32_t slot = 0) const;
		void BindCS(uint32_t slot = 0) const;

		void Set(const void*);

	private:

		ID3D11Buffer* mConstantBuffer = nullptr;
	};

	template <class DataType>
	class TypedConstantBuffer : public ConstantBuffer
	{
	public:

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type must be 16 byte aligned!");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Initialize(const DataType& data)
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type must be 16 byte aligned!");
			ConstantBuffer::Initialize(sizeof(DataType), &data);
		}

		void Set(const DataType& data)
		{
			ConstantBuffer::Set(&data);
		}
	};

}; // namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_CONSTANTBUFFER_H