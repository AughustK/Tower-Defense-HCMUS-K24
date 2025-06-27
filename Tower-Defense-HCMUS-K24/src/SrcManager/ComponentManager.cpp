#include "../../header/Managers/ComponentManager.h"

void ComponentManager::removeEntityComponent(EntityID entity)
{
	for (auto& des : componentArrays)
	{
		des.second->entityDestroyed(entity);
	}
}
