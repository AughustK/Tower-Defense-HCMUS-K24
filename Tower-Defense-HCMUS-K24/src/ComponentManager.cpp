#include "../header/Managers/ComponentManager.h"

using std::unordered_map;
using std::make_unique;
using std::type_index;

template <typename T>
inline void ComponentManager::removeComponent(EntityID entityID)
{
	auto& componentArray = getComponentArray<T>();  
	componentArray.removeData(entityID);
}

void ComponentManager::removeEntityComponent(EntityID entity)
{
	for (auto& des : componentArrays)
	{
		des.second->entityDestroyed(entity);
	}
}
