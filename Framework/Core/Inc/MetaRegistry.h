//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_SIEGE_CORE_METAREGISTRY_H
#define INCLUDED_SIEGE_CORE_METAREGISTRY_H

namespace Klink::Core::Meta
{
	class MetaClass;

	void Register(const MetaClass* metaClass);

	const MetaClass* FindMetaClass(const std::string& className);
}

#define META_REGISTER(Class)\
	Klink::Core::Meta::Register(Class::StaticGetMetaClass());

#endif // #ifndef INCLUDED_SIEGE_CORE_METAREGISTRY_H
