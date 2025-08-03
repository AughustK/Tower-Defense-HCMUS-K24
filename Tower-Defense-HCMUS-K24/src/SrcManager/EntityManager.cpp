#include "../../header/Managers/EntityManager.h"
#include <iostream>

EntityManager::EntityManager() noexcept
{
    for (EntityID id = 0; id < MAX_ENTITIES; ++id) 
    {
        availableEntities.push(id);
    }
    livingEntity = 0;
}

EntityID EntityManager::createEntity()
{
    assert(!availableEntities.empty() && "Too many entities");

	// Get the first available entity ID
    EntityID id = availableEntities.front();  
    availableEntities.pop();
    ++livingEntity;
    return id;
}

void EntityManager::destroyEntity(EntityID entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");

    //clear signature
    signatures[entity].reset();

    //make ID available immediately
    availableEntities.push(entity);
	std::cout << "[EntityManager] Entity " << entity << " is now available for reuse\n";
    //adjust count
    --livingEntity;
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

