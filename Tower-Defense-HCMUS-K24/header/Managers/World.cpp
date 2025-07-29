#include "World.h"
#include "../Components/UITextComponent.h"
#include "../Components/UISpriteComponent.h"
#include "../Components/SoundComponent.h"
#include "../Components/MusicComponent.h"
#include "../Components/UISliderComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PositionComponent.h"
#include"../Components/ClickComponent.h"
#include "../Components/PathfindingComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/Buffs.h"
#include "../Components/CircleComponent.h"
#include "../Components/TowerComponent.h"
#include "../Components/ShopComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CastleHPComponent.h"

#include "../Systems/SpriteRenderSystem.h"
#include "../Systems/TextRenderSystem.h"
#include "../Systems/System.h"
#include "../Systems/SoundSystem.h"
#include "../Systems/UISliderSystem.h"
#include "../Systems/MusicSystem.h"
#include"../Systems/PhysicSystem.h"
#include "../Systems/PathFindingSystem.h"
#include "../Systems/InitializeEnemy.h"
#include "../Systems/InitializeProjectile.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/TowerSystem.h"
#include "../GameStates/GamePlay.h"
#include "../Systems/CastleHPSystem.h"

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

    //set up for sound
    registerComponent<SoundComponent>();
    auto soundSystem = registerSystem<SoundSystem>();
    Signature soundSig;
    soundSig.set(getComponentType<SoundComponent>(), true);
    setSystemSignature<SoundSystem>(soundSig);

    //set up for music
    registerComponent<MusicComponent>();
    auto musicSystem = registerSystem<MusicSystem>();
    Signature musicSig;
    musicSig.set(getComponentType<MusicComponent>(), true);
    setSystemSignature<MusicSystem>(musicSig);

    //set up for volume
    registerComponent<SliderComponent>();
    auto sliderSystem = registerSystem<SliderSystem>();
    Signature sliderSig;
    sliderSig.set(getComponentType<SliderComponent>(), true);
    setSystemSignature<SliderSystem>(sliderSig);

    //set up for physic
    registerComponent<PositionComponent>();
    registerComponent<VelocityComponent>();
    auto physicSystem = registerSystem<PhysicSystem>();
    Signature physicSig;
    physicSig.set(getComponentType<PositionComponent>(), true);
    physicSig.set(getComponentType<VelocityComponent>(), true);
    setSystemSignature<PhysicSystem>(physicSig);

    //set up for click
    registerComponent<ClickComponent>();

    //set up for enemy spawning
    registerComponent<EnemyComponent>();
    registerComponent<PathComponent>();
    auto spawnSystem = registerSystem<EnemySpawnSystem>();
    Signature spawnSig;
    spawnSig.set(getComponentType<EnemyComponent>(), true);
    spawnSig.set(getComponentType<PathComponent>(), true);
    setSystemSignature<EnemySpawnSystem>(spawnSig);

    //set up for path system
    auto pathSystem = registerSystem<PathFollowingSystem>();
    Signature pathSig;
    pathSig.set(getComponentType<PathComponent>(), true);
    pathSig.set(getComponentType<PositionComponent>(), true);
    pathSig.set(getComponentType<VelocityComponent>(), true);
    setSystemSignature<PathFollowingSystem>(pathSig);

    //set up for collision
    registerComponent<HealthComponent>();
    registerComponent<BuffComponent>();
    registerComponent<CircleComponent>();
    registerComponent<TowerComponent>();
    registerComponent<ProjectileComponent>();

    auto colSystem = registerSystem<CollisionSystem>();
    Signature colSig;
    colSig.set(getComponentType<VelocityComponent>(), true);
    colSig.set(getComponentType<CircleComponent>(), true);
    colSig.set(getComponentType<ProjectileComponent>(), true);
    colSig.set(getComponentType<TowerComponent>(), true);
    colSig.set(getComponentType<HealthComponent>(), true);
    colSig.set(getComponentType<BuffComponent>(), true);
    setSystemSignature<CollisionSystem>(colSig);

    //set up for tower spawning
    registerComponent<TowerIconComponent>();

    //tower system
    auto towerSystem = registerSystem<TowerSystem>();
    Signature towerSig;
    towerSig.set(getComponentType<TowerComponent>(), true);
    towerSig.set(getComponentType<PositionComponent>(), true);
    towerSig.set(getComponentType<VelocityComponent>(), true);
    towerSig.set(getComponentType<CircleComponent>(), true);
    setSystemSignature<TowerSystem>(towerSig);

    //set up for projectile spawning
    auto projectileSystem = registerSystem<ProjectilePoolSystem>();
    Signature projectileSig;
    projectileSig.set(getComponentType<ProjectileComponent>(), true);
    projectileSig.set(getComponentType<PositionComponent>(), true);
    projectileSig.set(getComponentType<VelocityComponent>(), true);
    projectileSig.set(getComponentType<CircleComponent>(), true);
    setSystemSignature<ProjectilePoolSystem>(projectileSig);

    //set up for castle health
    registerComponent<CastleHPComponent>();
    auto castleHPSystem = registerSystem<CastleHPSystem>();
    Signature hpSig;
    hpSig.set(getComponentType<CastleHPComponent>(), true);
    setSystemSignature<CastleHPSystem>(hpSig);
}

EntityID World::createEntity()
{
    return entityManager.createEntity();
}

void World::destroyEntity(EntityID entityID)
{
    // First, remove the entity from all systems
    systemManager.removeEntitySystem(entityID);
    
    // Then remove all components
    componentManager.removeEntityComponent(entityID);
    
    // Finally destroy the entity in the entity manager
    entityManager.destroyEntity(entityID);
    
    // Now it's safe to make the entity ID available for reuse
    // This ensures all cleanup is complete before the ID can be reused
    entityManager.makeEntityAvailableForReuse(entityID);
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
    currentState->update(*this, deltaTime);
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