#ifndef INCLUDED_KLINK_WORLD_H
#define INCLUDED_KLINK_WORLD_H

#include "GameObject.h"
#include "GameObjectFactory.h"

#include "Service.h"

namespace Klink
{

	class World
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		template<class T, class = std::void_t<std::is_base_of<Service, T>>>
		T* AddService()
		{
			ASSERT(!mInitialized, "[World] Cannot add service after initialization");
			auto& newService = mServices.emplace_back(std::make_unique<T>());
			newService->mWorld = this;
			return static_cast<T*>(newService.get());
		}

		template <class T>
		T* AddService()
		{
			return const_cast<T*>(static_cast <const World*>(this)->GetService<T>());
		}

		template <class T>
		const T* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetMetaClass() == T::StaticGetMetaClass())
					return static_cast<T*>(service.get());
				return nullptr;
			}
		}

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		friend class Editor;

		void DestroyInternal(GameObject* gameObject);
		void ProcessDestroyList();

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
		std::unique_ptr<GameObjectFactory> mGameObjectFactory;

		GameObjectList mUpdateList;
		GameObjectList mDestroyList;

		bool mInitialized = false;
		bool mUpdating = false;
	};

}; //  namespace Klink

#endif // INCLUDED_KLINK_WORLD_H