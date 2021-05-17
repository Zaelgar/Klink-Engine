#include "Precompiled.h"
#include "MetaRegistration.h"
/*
namespace Klink::Core::JMath
{
	template <>
	void Deserialize<Vector2>(void* instance, const rapidjson::Value & jsonvalue)
	{
		auto object = value.GetObjectW();
		auto vec2 = (Vector2*)(instance);
		vec2->x = object.FindMember("x")->value.GetFloat();
		vec2->y = object.FindMember("y")->value.GetFloat();
	}

	template <>
	void Deserialize<Vector3>(void* instance, const rapidjson::Value & jsonvalue)
	{
		auto object = value.GetObjectW();
		auto vec3 = (Vector3*)(instance);
		vec3->x = object.FindMember("x")->value.GetFloat();
		vec3->y = object.FindMember("y")->value.GetFloat();
		vec3->z = object.FindMember("z")->value.GetFloat();
	}

	template <>
	void Deserialize<Vector4>(void* instance, const rapidjson::Value & jsonvalue)
	{
		auto object = value.GetObjectW();
		auto vec4 = (Vector4*)(instance);
		vec4->x = object.FindMember("x")->value.GetFloat();
		vec4->y = object.FindMember("y")->value.GetFloat();
		vec4->z = object.FindMember("z")->value.GetFloat();
		vec4->w = object.FindMember("w")->value.GetFloat();
	}
	
	template <>
	void Deserialize<Quaternion>(void* instance, const rapidjson::Value & jsonvalue)
	{
		auto object = value.GetObjectW();
		auto quaternion = (Quaternion*)(instance);
		quaternion->x = object.FindMember("x")->value.GetFloat();
		quaternion->y = object.FindMember("y")->value.GetFloat();
		quaternion->z = object.FindMember("z")->value.GetFloat();
		quaternion->w = object.FindMember("w")->value.GetFloat();
	}
}


META_TYPE_DEFINE(Klink::JMath::Vector2, "Vector2")
META_TYPE_DEFINE(Klink::JMath::Vector3, "Vector3")
META_TYPE_DEFINE(Klink::JMath::Vector4, "Vector4")
META_TYPE_DEFINE(Klink::JMath::Quaternion, "Quaternion")
META_TYPE_DEFINE(Klink::JMath::Matrix3, "Matrix3")
META_TYPE_DEFINE(Klink::JMath::Matrix4, "Matrix4")
*/
void Klink::JMath::StaticMetaRegister()
{
	
}