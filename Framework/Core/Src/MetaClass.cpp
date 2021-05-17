#include "Precompiled.h"
#include "MetaClass.h"

#include "Debug.h"
#include "MetaField.h"
#include "MetaRegistry.h"

using namespace Klink::Core::Meta;

MetaClass::MetaClass(
	const char* name,
	size_t size,
	MetaType::CreateFunc create,
	MetaType::DestroyFunc destroy,
	MetaType::SerializeFunc serialize,
	MetaType::DeserializeFunc deserialize,
	const MetaClass* parent,
	std::vector<MetaField> fields)
	: MetaType(MetaType::Category::Class, name, size, std::move(create), std::move(destroy), std::move(serialize), std::move(deserialize))
	, mParent(parent)
	, mFields(std::move(fields))
{}

void MetaType::Serialize(const void* instance, rapidjson::Value& jsonValue) const
{
	ASSERT(mSerialize, "[MetaType] no serialize callable registered for %s", GetName());
	mSerialize(instance, jsonValue);
}

const MetaClass* MetaClass::GetParent() const
{
	return mParent;
}

const MetaField* MetaClass::FindField(const char* name) const
{
	for (auto& field : mFields)
	{
		if (strcmp(field.GetName(), name) == 0)
			return &field;
	}

	// If the field is not found, try the parent
	if (mParent != nullptr)
		return mParent->FindField(name);

	return nullptr;
}

const MetaField* MetaClass::GetField(size_t index) const
{
	ASSERT(index < GetFieldCount(), "[MetaClass] Subscript out of range!");
	const size_t parentCount = GetParentFieldCount();
	if (index < parentCount)
		return mParent->GetField(index);
	return mFields.data() + (index - parentCount);
}

size_t MetaClass::GetFieldCount() const
{
	return mFields.size() + GetParentFieldCount();
}

size_t MetaClass::GetParentFieldCount() const
{
	return mParent ? mParent->GetFieldCount() : 0u;
}

void MetaClass::Serialize(const void* instance, rapidjson::Value& jsonvalue) const
{
	ASSERT(false, "To Do...");
}

/*
void MetaClass::Deserialize(void* instance, rapidjson::Value& jsonValue) const
{
	for (auto& member : jsonValue.GetObjectW())
	{
		auto metaField = FindField(member.name.GetString());
		auto metaType = metaField->GetMetaType();
		auto instanceField = static_cast<uint8_t*>(instance) + metaField->GetOffset();
		metaType->Deserialize(instanceField, jsonValue);
	}
}
*/