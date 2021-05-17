#ifndef INCLUDED_KLINK_GAMEOBJECTFACTORY_H
#define INCLUDED_KLINK_GAMEOBJECTFACTORY_H

#include "GameObject.h"

namespace Klink
{

class GameObjectFactory
{
public:
	GameObjectFactory(GameObjectAllocator& allocator);

	GameObject* Create(const char* templateFileName);
	void Destroy(GameObject* gameObject);

private:
	GameObjectAllocator& mGameObjectAllocator;
};

};

#endif // INCLUDED_KLINK_GAMEOBJECTFACTORY_H