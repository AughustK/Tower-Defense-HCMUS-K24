#include "World.h"

using std::shared_ptr;

World::World()
{
	systemManager.init(&entityManager);
}

EntityID World::createEntity()
{
	return entityManager.createEntity();
}

void World::destroyEntity(EntityID entityID)
{
	entityManager.destroyEntity(entityID);
	componentManager.removeEntityComponent(entityID);
	systemManager.removeEntitySystem(entityID);
}

template <typename T>
void World::registerComponent()
{
    componentManager.registerComponent();
}

template<typename T>
void World::addComponent(EntityID entityID, const T& component) 
{
    componentManager.addComponent<T>(entityID, component);

    // Update entity signature
    ComponentID compID = componentManager.getComponentType<T>();
    Signature signature = entityManager.getSignature(entityID);
    signature.set(compID, true);
    entityManager.setSignature(entity, signature);

    // Notify systems that this entity’s signature changed
    systemManager.entitySignatureChanged(entity, signature);
}

template<typename T>
void World::removeComponent(EntityID entity) 
{
    componentManager.removeComponent<T>(entity);

    ComponentID compID = componentManager.getComponentType<T>();
    Signature signature = entityManager.getSignature(entity);
    signature.set(compID, false);
    entityManager.setSignature(entity, signature);

    systemManager.entitySignatureChanged(entity, signature);
}

template<typename T>
T& World::getComponent(EntityID entity) {
    return componentManager.getComponentArray<T>().getData(entity);
}

template<typename T>
shared_ptr<T> World::registerSystem() 
{
    return systemManager.registerSystem<T>();
}

template<typename T>
void World::setSystemSignature(Signature signature) 
{
    systemManager.setSignature<T>(signature);
}

void World::update(float deltaTime) 
{
    for (auto const& pair : systemManager.getSystems()) 
    {
        pair.second->update(deltaTime);
    }
}





