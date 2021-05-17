#ifndef INCLUDED_KLINK_CORE_METATYPE_H
#define INCLUDED_KLINK_CORE_METATYPE_H

#include "Debug.h"
#include "RapidJSON/rapidjson.h"

namespace Klink::Core::Meta
{
	class MetaArray;
	class MetaClass;
	class MetaPointer;

	class MetaType
	{
	public:
		using CreateFunc = std::function<void*()>;
		using DestroyFunc = std::function<void(void*)>;
		using SerializeFunc = std::function<void(const void* instance, rapidjson::Value& jsonValue)>;
		using DeserializeFunc = std::function<void(const void* instance, const rapidjson::Value& jsonValue)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(
			Category category,
			const char* name,
			size_t size,
			CreateFunc create,
			DestroyFunc destroy,
			SerializeFunc serialize,
			DeserializeFunc deserialize);

		Category GetCategory() const { return mCategory; }
		const char* GetName() const { return mName.c_str(); }
		size_t GetSize() const { return mSize; }

		void* Create() const;
		void Destroy(void* data) const;
		virtual void Serialize(const void* instance, rapidjson::Value& jsonValue) const;
		virtual void Deserialize(const void* instance, rapidjson::Value& jsonValue) const;

	private:
		const Category mCategory;
		const std::string mName;
		const size_t mSize;
		const CreateFunc mCreate;
		const DestroyFunc mDestroy;
		const SerializeFunc mSerialize;
		const DeserializeFunc mDeserialize;
	};
}

#endif // #ifndef INCLUDED_KLINK_CORE_METATYPE_H
