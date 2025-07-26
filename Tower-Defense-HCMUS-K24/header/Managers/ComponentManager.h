#pragma once

#include "EntityManager.h"           
#include "ComponentArray.h"        
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cassert>

using std::unordered_map;
using std::unique_ptr;
using std::type_index;


class ComponentManager
{
private:
	// Map from component type to component array
	unordered_map<type_index, unique_ptr<IComponentArray>> componentArrays;
	unordered_map<type_index, ComponentID> componentTypes;
	ComponentID nextType = 0;
public:
	template <typename T>
	void registerComponent()
	{
		auto typeId = type_index(typeid(T));
		assert(componentTypes.find(typeId) == componentTypes.end() && "Component type already registered.");
		componentTypes[typeId] = nextType++;
		componentArrays[typeId] = std::make_unique<ComponentArray<T>>();
	}

	template<typename T>
	ComponentArray<T>& getComponentArray()
	{
		auto typeId = type_index(typeid(T));
		auto it = componentArrays.find(typeId);
		assert(it != componentArrays.end() && "Component not registered.");
		return *static_cast<ComponentArray<T>*>(it->second.get());
	}

	template <typename T>
	ComponentArray<T>& getComponentArray() const
	{
		auto typeId = type_index(typeid(T));
		auto it = componentArrays.find(typeId);
		assert(it != componentArrays.end() && "Component not registered");
		return *static_cast<ComponentArray<T>*>(it->second.get());
	}

	template <typename T>
	void addComponent(EntityID entityID, const T& component)
	{
		getComponentArray<T>().insertData(entityID, component);
	}

	template <typename T>
	void removeComponent(EntityID entityID)
	{
		getComponentArray<T>().removeData(entityID);
	}

	void removeEntityComponent(EntityID entity);

	template<typename T>
	ComponentID getComponentType() const
	{
		return componentTypes.at(type_index(typeid(T)));
	}

	template<typename T>
	vector<EntityID> getEntitiesWithComponent() 
	{
		vector<EntityID> result;
		auto& componentArray = getComponentArray<T>();
		for (const auto& pair : componentArray.getEntityToIndexMap()) {
			result.push_back(pair.first);  // EntityID
		}
		return result;
	}
}; 


