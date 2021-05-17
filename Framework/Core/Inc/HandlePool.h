#ifndef INCLUDED_KLINK_CORE_HANDLEPOOL_H
#define INCLUDED_KLINK_CORE_HANDLEPOOL_H

#include "Common.h"
#include "Debug.h"

namespace Klink::Core
{
	template <class DataType>
	class Handle;

	template <class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;

		HandlePool(uint32_t capacity);
		~HandlePool();

		HandleType Register(DataType* instance);
		void Unregister(HandleType handle);
		void Flush();

		bool IsValid(HandleType handle) const;
		DataType* Get(HandleType handle);

	private:
		struct Entry
		{
			DataType* instance = nullptr;
			uint32_t generation = 0;
		};

		std::vector<Entry> mEntries;
		std::vector<uint32_t> mFreeSlots;
	};

	template <class DataType>
	HandlePool<DataType>::HandlePool(uint32_t capacity)
	{
		ASSERT(capacity > 0, "[HandlePool] Invalid capacity!");
		mEntries.resize(capacity + 1);
		mFreeSlots.reserve(capacity);
		for (size_t i = capacity; i > 0; --i)
			mFreeSlots.push_back(static_cast<uint32_t>(i));

		// Register itself with the handle type
		ASSERT(HandleType::sPool == nullptr, "[HandlePool] Cannot have more than one pool for the same type!");
		HandleType::sPool = this;
	}

	template <class DataType>
	HandlePool<DataType>::~HandlePool()
	{
		// Check that all handles are freed
		ASSERT(mFreeSlots.size() == mFreeSlots.capacity(), "[HandlePool] There are still registered slots!");

		// Unregister itself with the handle type
		ASSERT(HandleType::sPool == this, "[HandlePool] Something is wrong.");
		HandleType::sPool = nullptr;
	}

	template <class DataType>
	Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
	{
		ASSERT(instance != nullptr, "[HandlePool] Invalid instance.");
		ASSERT(!mFreeSlots.empty(), "[HandlePool] No more free slots available.");

		// Draw the next free slot
		uint32_t slot = mFreeSlots.back();
		mFreeSlots.pop_back();

		// Register the instance
		mEntries[slot].instance = instance;

		// Return a handle to this entry
		HandleType handle;
		handle.mIndex = slot;
		handle.mGeneration = mEntries[slot].generation;
		return handle;
	}

	template <class DataType>
	void HandlePool<DataType>::Unregister(HandleType handle)
	{
		if (!IsValid(handle))
			return;

		// Increment generation to invalidate all existing handles
		mEntries[handle.mIndex].generation++;

		// Recycle the slot
		mFreeSlots.push_back(handle.mIndex);
	}

	template <class DataType>
	void HandlePool<DataType>::Flush()
	{
		// Force increment generation and invalidate all existing handles
		for (auto& entry : mEntries)
			entry.generation++;

		// Reclaim all slots
		mFreeSlots.clear();
		for (size_t i = mFreeSlots.capacity(); i > 0; --i)
			mFreeSlots.push_back(static_cast<uint32_t>(i));
	}

	template <class DataType>
	bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
	}

	template <class DataType>
	DataType* HandlePool<DataType>::Get(HandleType handle)
	{
		return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
	}
}

#endif // INCLUDED_KLINK_CORE_HANDLEPOOL_H