#include "Precompiled.h"
#include "Common.h"
#include "Debug.h"

#include "BlockAllocator.h"

using namespace Klink::Core;

namespace
{
	inline void* GetBlock(void* data, int index, size_t blockSize)
	{
		return static_cast<uint8_t*>(data) + (index * blockSize);
	}

	inline intptr_t& GetAsIntPtr(void* data, int index, size_t blockSize)
	{
		return *(reinterpret_cast<intptr_t*>(GetBlock(data, index, blockSize)));
	}

	inline intptr_t ToIntPtr(void* ptr)
	{
		return reinterpret_cast<intptr_t>(ptr);
	}

	inline void* ToPtr(intptr_t intptr)
	{
		return reinterpret_cast<void*>(intptr);
	}
}

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mData(nullptr)
	, mBlockSize(std::max(blockSize, sizeof(size_t)))
	, mCapacity(capacity)
	, mFreeSlot(0)
{
	ASSERT(capacity > 0, "[BlockAllocator] Capacity must be more than 0");

	// Preallocate memory
	mData = std::malloc(blockSize * capacity);
	mFreeSlot = ToIntPtr(mData);

	// Initialize free slots
	size_t i = 0;
	for (; i + 1 < capacity; ++i)
		GetAsIntPtr(mData, i, mBlockSize) = ToIntPtr(GetBlock(mData, i + 1, blockSize));
	GetAsIntPtr(mData, i, mBlockSize) = ToIntPtr(nullptr);
}

BlockAllocator::~BlockAllocator()
{
	std::free(mData);
}

void* BlockAllocator::Allocate()
{
	if (mFreeSlot == ToIntPtr(nullptr))
		return nullptr;

	// Track the next free slot
	void* nextSlot = ToPtr(mFreeSlot);
	mFreeSlot = *(intptr_t*)nextSlot;
	return nextSlot;
}

void BlockAllocator::Free(void* ptr)
{
	ptrdiff_t diff = static_cast<uint8_t*>(ptr) - static_cast<uint8_t*>(mData);
	if (diff < 0)
		return;

	uint32_t slot = static_cast<size_t>(diff) / mBlockSize;
	if (slot >= mCapacity)
		return;

	// Swap next free slot index
	GetAsIntPtr(mData, slot, mBlockSize) = mFreeSlot;
	mFreeSlot = ToIntPtr(ptr);
}