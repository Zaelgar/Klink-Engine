#ifndef INCLUDED_KLINK_CORE_METACLASS_H
#define INCLUDED_KLINK_CORE_METACLASS_H

#include "MetaType.h"

namespace Klink::Core::Meta
{
	class MetaField;

	class MetaClass : public MetaType
	{
	public:
		MetaClass(
			const char* name,
			size_t size,
			MetaType::CreateFunc create,
			MetaType::DestroyFunc destroy,
			MetaType::SerializeFunc serialize,
			MetaType::DeserializeFunc deserialize,
			const MetaClass* parent,
			std::vector<MetaField> fields);

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name) const;
		const MetaField* GetField(size_t index) const;
		size_t GetFieldCount() const;

		void Serialize(const void* instance, rapidjson::Value& jsonValue) const override;
		//void Deserialize(void* instance, rapidjson::Value& jsonValue) const override;

	private:
		size_t GetParentFieldCount() const;

		const MetaClass* mParent;
		const std::vector<MetaField> mFields;
	};
}

#endif // #ifndef INCLUDED_KLINK_CORE_METACLASS_H
