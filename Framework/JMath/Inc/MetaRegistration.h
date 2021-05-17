#ifndef INCLUDED_KLINK_JMATH_METAREGISTRATION_H
#define INCLUDED_KLINK_JMATH_METAREGISTRATION_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Quaternion.h"

#include "Matrix3.h"
#include "Matrix4.h"

// Primitive Type Declarations
META_TYPE_DECLARE(Klink::JMath::Vector2)
META_TYPE_DECLARE(Klink::JMath::Vector3)
META_TYPE_DECLARE(Klink::JMath::Vector4)
META_TYPE_DECLARE(Klink::JMath::Quaternion)
META_TYPE_DECLARE(Klink::JMath::Matrix3)
META_TYPE_DECLARE(Klink::JMath::Matrix4)

namespace Klink::JMath
{
	void StaticMetaRegister();
};

#endif // INCLUDED_KLINK_CORE_METAREGISTRATION_H