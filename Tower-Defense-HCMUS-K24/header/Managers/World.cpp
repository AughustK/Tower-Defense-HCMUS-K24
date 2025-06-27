#include "World.h"
#include "../Components/UITextComponent.h"
#include "../Components/UISpriteComponent.h"
#include "../Systems/SpriteRenderSystem.h"
#include "../Systems/TextRenderSystem.h"
#include "../Systems/System.h"
#include <iostream>

// Reserve 512KB for per-frame transient storage
static uint8_t gTransientStorage[512 * 1024];
Arena World::transientArena;


World::World() : window(sf::VideoMode(1920, 1080), "Tower Defense")
{
    // Initialize the bump allocator
    transientArena.init(gTransientStorage, sizeof(gTransientStorage));
    systemManager.init(&entityManager);
}

void World::init()
{
    std::cout << "Init called\n";
    systemManager.init(&entityManager);

    //set up for sprite
    registerComponent<SpriteComponent>();
    auto spriteRenderSystem = registerSystem<SpriteRenderSystem>();
    Signature renderSig0;
    renderSig0.set(getComponentType<SpriteComponent>(), true);
    setSystemSignature<SpriteRenderSystem>(renderSig0);

    //set up for text
    registerComponent<TextComponent>();
    auto textRenderSystem = registerSystem<TextRenderSystem>();
    Signature renderSig1;
    renderSig1.set(getComponentType<TextComponent>(), true);
    setSystemSignature<TextRenderSystem>(renderSig1);
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

void World::setState(std::unique_ptr<GameState> state)
{
    std::cout << "[World] Switching state to: " << typeid(*state).name() << '\n';
    if (currentState)
    {
        currentState->onExit(*this);
    }
    currentState = std::move(state);

    if (currentState)
    {
        currentState->onEnter(*this);
    }
}

void World::handleEvent(sf::Event& event)
{
    if (currentState) currentState->handleEvent(*this, event);
}

void World::render()
{
    if (currentState) currentState->render(*this, window);
}

const std::unique_ptr<GameState>& World::getCurrentState() const
{
    return currentState;
}

EntityManager World::getEntityManager()
{
    return entityManager;
}

SystemManager World::getSystemManager()
{
    return systemManager;
}

void World::update(float deltaTime)
{
    // Reset transient arena at the start of each frame
    transientArena.reset();

    for (auto const& pair : systemManager.getSystems())
    {
        pair.second->update(deltaTime);
    }
}

void World::requestClose()
{
    shouldClose = true;
}

bool World::isRunning() const
{
    return window.isOpen() && !shouldClose;
}


