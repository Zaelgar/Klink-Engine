#ifndef INCLUDED_KLINK_CORE_BLOCKALLOCATOR_H
#define INCLUDED_KLINK_CORE_BLOCKALLOCATOR_H

#include "Common.h"

namespace Klink::Core
{

	class BlockAllocator
	{
	public:
		BlockAllocator(size_t blockSize, size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;

		BlockAllocator(BlockAllocator&&) = delete;
		BlockAllocator& operator=(BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:
		void* mData;
		size_t mBlockSize;
		size_t mCapacity;
		intptr_t mFreeSlot;
	};

}

#endif // #ifndef INCLUDED_KLINK_CORE_BLOCKALLOCATOR_H