#ifndef INCLUDED_KLINK_CORE_METAREGISTRATION_H
#define INCLUDED_KLINK_CORE_METAREGISTRATION_H

#include "Meta.h"

// Primitive Type Declarations
META_TYPE_DECLARE(int)
META_TYPE_DECLARE(float)
META_TYPE_DECLARE(bool)
META_TYPE_DECLARE(std::string)

namespace Klink::Core
{
	void StaticMetaRegister();
};

#endif // INCLUDED_KLINK_CORE_METAREGISTRATION_H