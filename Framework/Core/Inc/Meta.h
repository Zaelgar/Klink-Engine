// References:
//	http://www.vollmann.com/en/pubs/meta/meta/meta.html
//	http://www.gamasutra.com/view/feature/134748/sponsored_feature_behind_the_.php
//	http://replicaisland.blogspot.ca/2010/11/building-reflective-object-system-in-c.html
//	http://www.gamasutra.com/view/news/128978/Reflection_in_C_The_simple_implementation_of_Splinter_Cell.php
//====================================================================================================

#ifndef INCLUDED_KLINK_CORE_META_H
#define INCLUDED_KLINK_CORE_META_H

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

namespace Klink::Core
{
#define META_TYPE_DECLARE(Type)\
	template <> const Klink::Core::Meta::MetaType* Klink::Core::Meta::DeduceType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template <> const Klink::Core::Meta::MetaType* Klink::Core::Meta::DeduceType<Type>()\
	{\
		static const Klink::Core::Meta::MetaType sMetaType(\
			Klink::Core::Meta::MetaType::Category::Primitive, #Name, sizeof(Type),\
			[] { return new Type; },\
			[](void* data) { delete static_cast<Type*>(data); });\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const Klink::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const Klink::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Klink::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Klink::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const Klink::Core::Meta::MetaType* Klink::Core::Meta::DeduceType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const Klink::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;

#define META_FIELD_BEGIN\
		static const std::initializer_list<Klink::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ Klink::Core::Meta::DeduceMemberType(&Class::Var), Name, Klink::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<Klink::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const Klink::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			[] { return new Class(); },\
			[](void* data) { delete static_cast<Class*>(data); },\
			parentMetaClass, fields);\
		return &sMetaClass;\
	}
}

#endif // #ifndef INCLUDED_KLINK_CORE_META_H
