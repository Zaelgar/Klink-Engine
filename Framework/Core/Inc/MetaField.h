//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#ifndef INCLUDED_SIEGE_CORE_METAFIELD_H
#define INCLUDED_SIEGE_CORE_METAFIELD_H

namespace Klink::Core::Meta
{
	class MetaType;

	class MetaField
	{
	public:
		MetaField(const MetaType* type, const char* name, size_t offset);

		const MetaType* GetMetaType() const { return mType; }
		const char* GetName() const { return mName.c_str(); }
		size_t GetOffset() const { return mOffset; }

	private:
		const MetaType* mType;
		const std::string mName;
		const size_t mOffset;
	};
}

#endif // #ifndef INCLUDED_SIEGE_CORE_METAFIELD_H
