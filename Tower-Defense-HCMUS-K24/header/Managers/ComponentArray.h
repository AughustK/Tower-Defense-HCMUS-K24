#pragma once

#include "EntityManager.h"
#include <array>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <cassert>
#include <iostream> // Added for debug output

using std::array;
using std::unordered_map;

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(EntityID entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
private:
	array<T, MAX_ENTITIES> componentArray;
	unordered_map<EntityID, uint32_t> entityToIndexMap;
	unordered_map<uint32_t, EntityID> indexToEntityMap;
	uint32_t size{};

public:
	void insertData(EntityID entity, const T& component)
	{
		assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");
		
		uint32_t newIndex = size;

		entityToIndexMap[entity] = newIndex; // Map entity to index
		indexToEntityMap[newIndex] = entity;  // Map index to entity
		componentArray[newIndex] = component;
		++size;
	}

	void removeData(EntityID entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");
		uint32_t deleteIndexEntity = entityToIndexMap[entity];
		uint32_t lastIndexElement = size - 1;

		componentArray[deleteIndexEntity] = componentArray[lastIndexElement]; // Move last element to deleted index
		EntityID lastElementEntity = indexToEntityMap[lastIndexElement];  // Get the entity of the last element
		entityToIndexMap[lastElementEntity] = deleteIndexEntity; // Update the entity to index map
		indexToEntityMap[deleteIndexEntity] = lastElementEntity; // Update the index to entity map


		entityToIndexMap.erase(entity);  // Remove the entity from the map
		indexToEntityMap.erase(lastIndexElement);

		--size;
	}

	T& getData(EntityID entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");
		return componentArray[entityToIndexMap[entity]];
	}

	bool containData(EntityID entity) const
	{
		return entityToIndexMap.find(entity) != entityToIndexMap.end();
	}

	void entityDestroyed(EntityID entity) override
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			removeData(entity);
		}
	}

	const unordered_map<EntityID, uint32_t>& getEntityToIndexMap() const
	{
		return entityToIndexMap;
	}

	uint32_t getSize() const
	{
		return size;
	}
};