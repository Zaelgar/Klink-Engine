//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_SIEGE_CORE_METAARRAY_H
#define INCLUDED_SIEGE_CORE_METAARRAY_H

#include "MetaType.h"

namespace Klink::Core::Meta
{
	class MetaArray : public MetaType
	{
	public:
		using GetElementFunc = std::function<void*(void*, size_t)>;
		using GetCountFunc = std::function<size_t(void*)>;

		MetaArray(const MetaType* elementType, GetElementFunc getElement, GetCountFunc getCount);

		const MetaType* GetElementType() const { return mElementType; }
		void* GetElement(void* instance, size_t index) const { return mGetElement(instance, index); }
		size_t GetCount(void* instance) const { return mGetCount(instance); }

	private:
		const MetaType* mElementType;
		GetElementFunc mGetElement;
		GetCountFunc mGetCount;
	};
}

#endif // #ifndef INCLUDED_SIEGE_CORE_METAARRAY_H
