#pragma once

#include "EntityManager.h"
#include <array>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <cassert>
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
	void insertData(EntityID entity, const T& component);
	void removeData(EntityID entity);
	T& getData(EntityID entity);
	bool containData(EntityID entity) const;
	void entityDestroyed(EntityID entity) override;
	const unordered_map<EntityID, uint32_t>& getEntityToIndexMap() const;
	uint32_t getSize() const;
};




