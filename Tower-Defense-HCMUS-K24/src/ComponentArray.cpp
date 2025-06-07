#include "../header/Managers/ComponentArray.h"

template<typename T>
void ComponentArray<T>::insertData(EntityID entity, const T& component)
{
	assert(componentArray.find(entity) == componentArray.end() && "Component added to same entity more than once.");
	uint32_t newIndex = size;

	entityToIndexMap[entity] = newIndex; // Map entity to index
	indexToEntityMap[newIndex] = entity;  // Map index to entity
	componentArray[newIndex] = component;
	++size;
}

template<typename T>
void ComponentArray<T>::removeData(EntityID entity)
{
	assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");
	uint32_t deleteIndexEntity = entityToIndexMap[entity];
	uint32_t lastIndexElement = size - 1;

	componentArray[deleteIndexEntity] = componentArray[lastIndexElement]; // Move last element to deleted index
	EntityID lastElementEntity = indexToEntityMap[lastIndexElement];  // Get the entity of the last element
	entityToIndexMap[lastElementEntity] = deleteIndexEntity; // Update the entity to index map
	indexToEntityMap[deleteIndexEntity] = lastElementEntity;

	entityToIndexMap.erase(entity);  // Remove the entity from the map
	indexToEntityMap.erase(lastIndexElement);

	--size;
}

template<typename T>
T& ComponentArray<T>::getData(EntityID entity)
{
	assert(entityToIndexMap.count(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");
	return componentArray[entityToIndexMap[entity]];
}

template<typename T>
bool ComponentArray<T>::containData(EntityID entity) const
{
	return entityToIndexMap.find(entity) != entityToIndexMap.end();
}

template<typename T>
void ComponentArray<T>::entityDestroyed(EntityID entity)
{
	if (componentArray.find(entity) != componentArray.end())
	{
		removeData(entity);
	}
}


template<typename T>
const unordered_map<EntityID, uint32_t>& ComponentArray<T>::getEntityToIndexMap() const
{
	return entityToIndexMap;
}

template<typename T>
uint32_t ComponentArray<T>::getSize() const
{
	return size;
}








