#ifndef INCLUDED_KLINK_H
#define INCLUDED_KLINK_H
#include "Common.h"

// App headers
#include "AppState.h"
#include "App.h"

// Component headers
#include "ColliderComponent.h"
#include "TransformComponent.h"

// GameObject headers
#include "GameObject.h"
#include "GameObjectFactory.h"


#include "World.h"

// Services
#include "Service.h"


namespace Klink { extern Klink::App sApp; }

namespace KlinkApp
{
	template <class T>
	void AddState(std::string name)
	{
		Klink::sApp.AddState<T>(name);
	}

	void ChangeState(std::string name);
	void Run(Klink::AppConfig appConfig);
	void Shutdown();
};

#endif // INCLUDED_KLINK_H