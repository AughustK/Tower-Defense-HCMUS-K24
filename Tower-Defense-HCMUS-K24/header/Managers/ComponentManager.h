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
	inline void registerComponent();

	template <typename T>
	inline void addComponent(EntityID entityID, const T& component);

	template <typename T>
	inline void removeComponent(EntityID entityID);

	template <typename T>
	inline ComponentArray<T>& getComponentArray();

	template <typename T>
	inline const ComponentArray<T>& getComponentArray() const;

	void removeEntityComponent(EntityID entity);

	template<typename T>
	inline ComponentID getComponentType() const;
}; 


