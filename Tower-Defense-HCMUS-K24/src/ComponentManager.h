#pragma once
#include "Component.h"
#include "EntityManager.h"
#include <array>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <cassert>

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
	array<T, MAX_ENTITIES> m_componentArray;
	unordered_map<EntityID, size_t> m_entityToIndexMap; 
	unordered_map<size_t, EntityID> m_indexToEntityMap; 
	size_t m_size{};

public:
	void insertData(EntityID entity, T component);
	void removeData(EntityID entity);
	T& getData(EntityID entity);
	bool containData(EntityID entity) const;
	void entityDestroyed(EntityID entity) override;
	T* getComponentArray();
	// Returns the entity to index mapping
	const unordered_map<EntityID, size_t>& GetEntityToIndexMap() const;
	size_t getSize() const;
};


