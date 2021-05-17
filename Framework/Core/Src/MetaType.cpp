#include "Precompiled.h"
#include "MetaType.h"

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaPointer.h"

using namespace Klink::Core::Meta;

MetaType::MetaType(MetaType::Category category, const char* name, size_t size, CreateFunc create, DestroyFunc destroy, SerializeFunc serialize, DeserializeFunc deserialize)
	: mCategory(category)
	, mName(name)
	, mSize(size)
	, mCreate(std::move(create))
	, mDestroy(std::move(destroy))
	, mSerialize(std::move(serialize))
	, mDeserialize(std::move(deserialize))
{}

void* MetaType::Create() const
{
	ASSERT(mCreate, "[MetaType] no creation callable registered for '%s'.", GetName());
	return mCreate();
}

void MetaType::Destroy(void* data) const
{
	ASSERT(mDestroy, "[MetaType] no destruction callable registered for '%s'.", GetName());
	mDestroy(data);
}

void MetaType::Serialize(const void* instance, rapidjson::Value& jsonValue) const
{
	ASSERT(mSerialize, "[MetaType] no serialize callable registered for %s", GetName());
	mSerialize(instance, jsonValue);
}

void MetaType::Deserialize(const void* instance, rapidjson::Value& jsonValue) const
{
	ASSERT(mDeserialize, "[MetaType] no deserialize callable registered for %s", GetName());
	mDeserialize(instance, jsonValue);
}