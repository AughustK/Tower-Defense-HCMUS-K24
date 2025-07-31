#include "../../header/Managers/EntityManager.h"

EntityManager::EntityManager() noexcept
{
    availableEntities.reserve(MAX_ENTITIES);  // Pre-allocate memory
    for (EntityID id = 0; id < MAX_ENTITIES; ++id) 
    {
        availableEntities.push_back(id);
    }
    livingEntity = 0;
}

EntityID EntityManager::createEntity()
{
    assert(!availableEntities.empty() && "Too many entities");
    EntityID id = availableEntities.back();  
    availableEntities.pop_back();
    ++livingEntity;
    return id;
}

void EntityManager::destroyEntity(EntityID entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    
    // Clear the entity's signature to ensure no stale component references
    signatures[entity].reset();
    
    // Don't immediately make entity available for reuse
    // This will be done by a separate call to makeEntityAvailableForReuse()
    --livingEntity;
}

void EntityManager::makeEntityAvailableForReuse(EntityID entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    
    // Double-check that the signature is cleared
    signatures[entity].reset();
    
    // Add to available entities for reuse
    availableEntities.push_back(entity);
}

void EntityManager::addComponent(EntityID entity, ComponentID component)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
	signatures[entity].set(component);
}

void EntityManager::setSignature(EntityID entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    signatures[entity] = signature;
}

Signature EntityManager::getSignature(EntityID entity) const
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    return signatures[entity];
}

uint32_t EntityManager::getLivingEntityCount() const noexcept
{
    return livingEntity;
}

