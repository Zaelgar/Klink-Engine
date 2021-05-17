//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_SIEGE_CORE_METAPOINTER_H
#define INCLUDED_SIEGE_CORE_METAPOINTER_H

#include "MetaType.h"

namespace Klink::Core::Meta
{
	class MetaPointer : public MetaType
	{
	public:
		MetaPointer(const MetaType* pointerType);

		const MetaType* GetPointerType() const { return mPointerType; }

	private:
		const MetaType* mPointerType;
	};
}

#endif // #ifndef INCLUDED_SIEGE_CORE_METAPOINTER_H
