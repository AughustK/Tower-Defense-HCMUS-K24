#include "ComponentManager.h"

template<typename T>
void ComponentArray<T>::insertData(EntityID entity, T component)
{
	assert(m_componentArray.find(entity) == m_componentArray.end() && "Component added to same entity more than once.");
	size_t newIndex = m_size;

	m_entityToIndexMap[entity] = newIndex; // Map entity to index
	m_indexToEntityMap[newIndex] = entity;  // Map index to entity
	m_componentArray[newIndex] = component;
	++m_size;
}

template<typename T>
void ComponentArray<T>::removeData(EntityID entity)
{
	assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");
	size_t deleteIndexEntity = m_entityToIndexMap[entity];
	size_t lastIndexElement = --m_size;

	m_componentArray[deleteIndexEntity] = m_componentArray[lastIndexElement]; // Move last element to deleted index
	EntityID lastElementEntity = m_indexToEntityMap[lastIndexElement];  // Get the entity of the last element
	m_entityToIndexMap[lastElementEntity] = deleteIndexEntity; // Update the entity to index map
	m_indexToEntityMap[deleteIndexEntity] = lastElementEntity;

	m_entityToIndexMap.erase(entity);  // Remove the entity from the map
	m_indexToEntityMap.erase(lastIndexElement);
	--m_size;
}

template<typename T>
T& ComponentArray<T>::getData(EntityID entity)
{
	assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");
	return *m_componentArray[m_entityToIndexMap[entity]];
}

template<typename T>
bool ComponentArray<T>::containData(EntityID entity) const
{
	return m_entityToIndexMap.find(entity) != m_entityToIndexMap.end();
}

template<typename T>
void ComponentArray<T>::entityDestroyed(EntityID entity)
{
	if (m_componentArray.find(entity) != m_componentArray.end())
	{
		RemoveData(entity);
	}
}

template<typename T>
T* ComponentArray<T>::getComponentArray()
{
	return m_componentArray.data();
}

template<typename T>
const unordered_map<EntityID, size_t>& ComponentArray<T>::GetEntityToIndexMap() const
{
	return m_entityToIndexMap;
}

template<typename T>
size_t ComponentArray<T>::getSize() const
{
	return m_size;
}








