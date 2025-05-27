#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <vector>
#include <array>
#include <bitset>
#include <cassert>
#include <algorithm>

using namespace std;

using EntityID = uint32_t;
using ComponentID = uint8_t;

constexpr EntityID MAX_ENTITIES = 1000;
constexpr ComponentID MAX_COMPONENTS = 32;

using Signature = bitset<MAX_COMPONENTS>;  // Track components for each entity

class EntityManager
{
private:
	vector<EntityID> m_availableEntities;
	array<Signature, MAX_ENTITIES> m_signatures;
	uint32_t m_livingEntity{};

public:
	EntityManager() noexcept;
	EntityID createEntity(); 
	void destroyEntity(EntityID entity);
	void setSignature(EntityID entity, Signature signature);
	Signature getSignature(EntityID entity) const;
	uint32_t getLivingEntityCount() const noexcept;
};

#endif // ENTITY_H
