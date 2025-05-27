#include "EntityManager.h"

EntityManager::EntityManager() noexcept
{
    m_availableEntities.reserve(MAX_ENTITIES);  // Pre-allocate memory
    for (EntityID id = 0; id < MAX_ENTITIES; ++id) 
    {
        m_availableEntities.push_back(id);
    }
    m_livingEntity = 0;
}

EntityID EntityManager::createEntity()
{
    assert(!m_availableEntities.empty() && "Too many entities");
    EntityID id = m_availableEntities.back();  
    m_availableEntities.pop_back();
    ++m_livingEntity;
    return id;
}

void EntityManager::destroyEntity(EntityID entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    m_signatures[entity].reset();
    m_availableEntities.push_back(entity); 
    --m_livingEntity;
}

void EntityManager::setSignature(EntityID entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    m_signatures[entity] = signature;
}

Signature EntityManager::getSignature(EntityID entity) const
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    return m_signatures[entity];
}

uint32_t EntityManager::getLivingEntityCount() const noexcept
{
	return m_livingEntity;
}

