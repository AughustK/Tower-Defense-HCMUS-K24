#include "../header/Managers/ComponentManager.h"

using std::unordered_map;
using std::make_unique;
using std::type_index;

template <typename T>
inline void ComponentManager::registerComponent()
{
	auto typeId = type_index(typeid(T));
	assert(componentTypes.find(typeId) == componentTypes.end() && "Component type already registered.");
	componentTypes[typeId] = nextType++;
	componentArrays[typeId] = make_unique<ComponentArray<T>>();
}

template<typename T>
inline ComponentID ComponentManager::getComponentType() const
{
	return componentTypes.at(type_index(typeid(T)));
}

template<typename T>
inline ComponentArray<T>& ComponentManager::getComponentArray()
{
	auto typeId = type_index(typeid(T));
	auto it = componentArrays.find(typeId);
	assert(it != componentArrays.end() && "Component not registered.");
	return *static_cast<ComponentArray<T>*>(it->second.get());
}

template <typename T>
inline const ComponentArray<T>& ComponentManager::getComponentArray() const
{
	auto typeId = type_index(typeid(T));
	auto it = componentArrays.find(typeId);
	assert(it != componentArrays.end() && "Component not registered");
	return *static_cast<const ComponentArray<T>*>(it->second.get());
}

template <typename T>
inline void ComponentManager::addComponent(EntityID entityID, const T& component)
{
	getComponentArray<T>.insertData(entityID, component);
}

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
