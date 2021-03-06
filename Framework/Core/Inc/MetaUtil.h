#ifndef INCLUDED_KLINK_CORE_METAUTIL_H
#define INCLUDED_KLINK_CORE_METAUTIL_H

#include "MetaArray.h"
#include "MetaPointer.h"

namespace Klink::Core::Meta
{
	class MetaType;

	template <class DataType>
	const MetaType* DeduceType();

	namespace Detail
	{
		template <class DataType>
		inline const MetaType* DeduceTypeImpl(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "Cannot deduce type.");
		}

		template <class DataType>
		inline const MetaType* DeduceTypeImpl(DataType**)
		{
			static const MetaPointer sMetaPointer(DeduceType<DataType>());
			return &sMetaPointer;
		}

		template <class DataType>
		inline const MetaType* DeduceTypeImpl(std::vector<DataType>*)
		{
			static const MetaArray sMetaArray(
				DeduceType<DataType>(),
				[](void* instance, size_t index) -> void*
				{
					auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
					vector->resize(index + 1);
					return vector->data() + index;
				},
				[](void* instance) -> size_t
				{
					auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
					return vector->size();
				});
			return &sMetaArray;
		}
	}

	template <class DataType>
	inline const MetaType* DeduceType()
	{
		return Detail::DeduceTypeImpl(static_cast<DataType*>(nullptr));
	}

	template <class ClassType, class DataType>
	inline const MetaType* DeduceMemberType(DataType ClassType::*)
	{
		return DeduceType<DataType>();
	}

	template <class ClassType, class DataType>
	inline size_t GetFieldOffset(DataType ClassType::* field)
	{
		return (size_t)(void*)&(((ClassType*)nullptr)->*field);
	}

	template <class BaseType, class DerivedType>
	inline DerivedType* DynamicCast(BaseType* ptr)
	{
		if (ptr->GetMetaClass() != DerivedType::StaticGetMetaClass())
			return nullptr;
		return static_cast<DerivedType*>(ptr);
	}

	template <class BaseType, class DerivedType>
	inline bool IsBaseOf()
	{
		auto parent = DerivedType::StaticGetMetaClass()->GetParent();
		while (parent)
		{
			if (parent == BaseType::StaticGetMetaClass())
				return true;
			parent = parent->GetParent();
		}
		return false;
	}

	/// Forward Declare??
	//template<class DataType>
	//void Deserialize<DataType>(void* instance, const rapidjson::Value& jsonValue);
}

#endif // #ifndef INCLUDED_KLINK_CORE_METAUTIL_H