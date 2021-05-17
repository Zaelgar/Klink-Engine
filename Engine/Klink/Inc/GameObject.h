#ifndef INCLUDED_KLINK_GAMEOBJECT_H
#define INCLUDED_KLINK_GAMEOBJECT_H

#include "Editor.h"

namespace Klink
{
	class Component;
	class GameObject;
	class World;

	using GameObjectAllocator = Core::TypedAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final
	{
	public:
		META_CLASS_DECLARE;

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		Component* AddComponent(const Core::Meta::MetaClass* metaClass);

		template <class T, class = std::void_t<std::is_base_of<Component, T>>>
		T* AddComponent()
		{
			auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}

		template <class T>
		T* GetComponent()
		{
			return const_cast<T*>(static_cast<const GameObject*>(this)->GetComponent<T>());
		}

		template <class T>
		const T* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetMetaClass() == T::StaticGetMetaClass())
					return static_cast<T*>(component.get());
			}
			return nullptr;
		}

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }

		void SetName(const char* name) { mName = name; }
		const char* GetName() const { return mName.c_str(); }

		GameObjectHandle GetHandle() const { return mHandle; }

	private:
		friend class Editor;
		friend class World;
		using Components = std::vector<std::unique_ptr<Component>>;

		World* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components mComponents;
	};
}

#endif // #ifndef INCLUDED_SIEGE_GAMEOBJECT_H