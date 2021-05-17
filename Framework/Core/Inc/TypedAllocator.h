#ifndef INCLUDED_KLINK_CORE_TYPEDALLOCATOR_H
#define INCLUDED_KLINK_CORE_TYPEDALLOCATOR_H

#include "BlockAllocator.h"

namespace Klink::Core
{

template <class DataType>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(size_t capacity);

	DataType* New()
	{
		void* mem = Allocate();
		if (mem != nullptr)
		{
			new(mem) DataType();
		}
		return reinterpret_cast<DataType*>(mem);
	}

	template <class... Args>
	DataType* New(Args&&... args);

	void Delete(DataType* ptr);
};


template <class DataType>
TypedAllocator<DataType>::TypedAllocator(size_t capacity)
	: BlockAllocator(sizeof(DataType), capacity)
{}

template <class DataType>
template <class... Args>
DataType* TypedAllocator<DataType>::New(Args&&... args)
{
	DataType* instance = static_cast<DataType*>(Allocate());
	if (instance)
		new (instance) DataType(std::forward<Args>(args)...);
	return instance;
}

template <class DataType>
void TypedAllocator<DataType>::Delete(DataType* ptr)
{
	if (ptr == nullptr)
		return;

	ptr->~DataType();
	Free(ptr);
}

}

#endif // INCLUDED_KLINK_CORE_TYPEDALLOCATOR_H