//====================================================================================================
// SIEGE - Super Innovative Exciting Game Engine
// Copyright (c) 2020 Peter Chan. All rights reserved.
//====================================================================================================

#include "Precompiled.h"
#include "MetaField.h"

using namespace Klink::Core::Meta;

MetaField::MetaField(const MetaType* type, const char* name, size_t offset)
	: mType(type)
	, mName(name)
	, mOffset(offset)
{}