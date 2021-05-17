#ifndef INCLUDED_KLINK_CORE_HANDLE_H
#define INCLUDED_KLINK_CORE_HANDLE_H

namespace Klink::Core
{
template <class DataType>
class HandlePool;

template <class DataType>
class Handle
{
public:
	Handle();

	bool IsValid() const;
	void Invalidate();

	DataType* Get() const;
	DataType* operator->() const;

	bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
	bool operator!=(Handle rhs) const { return !(*this == rhs); }

private:
	friend class HandlePool<DataType>;

	static HandlePool<DataType>* sPool;

	uint32_t mIndex : 16;
	uint32_t mGeneration : 16;
};

template <class DataType>
HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

template <class DataType>
Handle<DataType>::Handle()
	: mIndex(0)
	, mGeneration(0)
{
}

template <class DataType>
bool Handle<DataType>::IsValid() const
{
	return sPool && sPool->IsValid(*this);
}

template <class DataType>
void Handle<DataType>::Invalidate()
{
	*this = Handle();
}

template <class DataType>
DataType* Handle<DataType>::Get() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

template <class DataType>
DataType* Handle<DataType>::operator->() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}
}

#endif // INCLUDED_KLINK_CORE_HANDLE_H


// Handle Pool
// if capacity is 100, allocate 101. slot 0 is never a free slot, its how we invalidate a handle.
// const Getters for unit testing