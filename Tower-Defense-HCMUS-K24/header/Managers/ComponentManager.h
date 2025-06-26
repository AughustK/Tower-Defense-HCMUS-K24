#pragma once

#include "EntityManager.h"           
#include "ComponentArray.h"        
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cassert>
#include <iostream>

using std::unordered_map;
using std::unique_ptr;
using std::type_index;
using std::make_unique;

class ComponentManager
{
private:
	// Map from component type to component array
	unordered_map<type_index, unique_ptr<IComponentArray>> componentArrays;
	unordered_map<type_index, ComponentID> componentTypes;
	ComponentID nextType = 0;
public:
	template <typename T>
	inline void registerComponent()
	{
		auto typeId = type_index(typeid(T));
		assert(componentTypes.find(typeId) == componentTypes.end() && "Component type already registered.");
		componentTypes[typeId] = nextType++;
		componentArrays[typeId] = make_unique<ComponentArray<T>>();

		std::cout << "Registered component: " << typeid(T).name()
			<< " with ID: " << static_cast<int>(componentTypes[typeId]) << '\n';
	}

	template <typename T>
	inline void addComponent(EntityID entityID, const T& component)
	{
		getComponentArray<T>().insertData(entityID, component);
	}

	template <typename T>
	inline void removeComponent(EntityID entityID);

	template <typename T>
	inline ComponentArray<T>& getComponentArray() const
	{
		auto typeId = type_index(typeid(T));
		auto it = componentArrays.find(typeId);
		assert(it != componentArrays.end() && "Component not registered");
		return *static_cast<ComponentArray<T>*>(it->second.get());
	}

	void removeEntityComponent(EntityID entity);

	template<typename T>
	inline ComponentID getComponentType() const
	{
		return componentTypes.at(type_index(typeid(T)));
	}

	template <typename T>
	std::vector<EntityID> getEntitiesWithComponent() {
		std::vector<EntityID> result;
		auto& componentArray = getComponentArray<T>();
		for (const auto& pair : componentArray.getEntityToIndexMap()) {
			result.push_back(pair.first);  // pair.first là EntityID
		}
		return result;
	}
}; 


