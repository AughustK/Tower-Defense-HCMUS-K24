#include "World.h"
#include "..\UIComponents\UITextComponent.h"
#include "..\UIComponents\UISpriteComponent.h"
#include "..\UISystems\SpriteRenderSystem.h"
#include "..\System.h"
#include <iostream>

using std::shared_ptr;

World::World() : window(sf::VideoMode(1920, 1080), "Tower Defense")
{
	systemManager.init(&entityManager);
}

void World::init() {
    std::cout << "Init called\n";
    systemManager.init(&entityManager);

    registerComponent<SpriteComponent>();

    auto renderSystem = registerSystem<SpriteRenderSystem>();

    Signature renderSig0;
    renderSig0.set(getComponentID<SpriteComponent>(), true);

    setSystemSignature<SpriteRenderSystem>(renderSig0);
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

void World::update(float deltaTime) 
{
    for (auto const& pair : systemManager.getSystems()) 
    {
        pair.second->update(deltaTime);
    }
}

EntityManager World::getEntityManager() {
    return entityManager;
}

SystemManager World::getSystemManager() {
    return systemManager;
}





