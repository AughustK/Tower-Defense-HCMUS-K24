#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <bitset>
#include <cassert>
#include <algorithm>

using std::vector;
using std::array;
using std::bitset;

using EntityID = uint32_t;
using ComponentID = uint8_t;
inline constexpr EntityID INVALID_ENTITY = std::numeric_limits<EntityID>::max();

constexpr EntityID MAX_ENTITIES = 2000;
constexpr ComponentID MAX_COMPONENTS = 32;

using Signature = bitset<MAX_COMPONENTS>;  // Track components for each entity

class EntityManager
{
private:
	vector<EntityID> availableEntities;
	array<Signature, MAX_ENTITIES> signatures;
	uint32_t livingEntity{};

public:
	EntityManager() noexcept;
	EntityID createEntity(); 
	void destroyEntity(EntityID entity);
	void makeEntityAvailableForReuse(EntityID entity);
	void addComponent(EntityID entity, ComponentID component);
	void setSignature(EntityID entity, Signature signature);
	Signature getSignature(EntityID entity) const;
	uint32_t getLivingEntityCount() const noexcept;
};


