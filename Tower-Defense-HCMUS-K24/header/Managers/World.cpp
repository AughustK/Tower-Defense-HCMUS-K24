#include "World.h"
#include "../GameStates/GamePlay.h"

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
#include "../Components/CircleComponent.h"
#include "../Components/TowerComponent.h"
#include "../Components/ShopComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CastleHPComponent.h"
#include "../Components/EnemyHPComponent.h"
#include "../Components/TagComponent.h"
#include "../Components/NoDimComponent.h"


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
#include "../Systems/CastleHPSystem.h"
#include "../Systems/EnemyHPSystem.h"


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
        registerComponent<EnemyHPComponent>();
        registerComponent<EnemyComponent>();
        registerComponent<PathComponent>();
        auto enemyHPSystem = registerSystem<EnemyHPSystem>();
        auto spawnSystem = registerSystem<EnemySpawnSystem>();
        Signature spawnSig;
        spawnSig.set(getComponentType<EnemyComponent>(), true);
        spawnSig.set(getComponentType<PathComponent>(), true);
        spawnSig.set(getComponentType<EnemyHPComponent>(), true);
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

        registerComponent<NoDimComponent>();

	    // Initialize the pool
        constexpr std::size_t MAX_ENEMIES = 200;
        constexpr std::size_t MAX_PROJECTILES = 250;

        registerComponent<TagComponent>();
        spawnSystem->initPool(*this, MAX_ENEMIES);
        projectileSystem->initPool(*this, MAX_PROJECTILES);
    }

EntityID World::createEntity()
{
    return entityManager.createEntity();
}

void World::destroyEntity(EntityID entityID)
{
    // Remove entity from all systems
    systemManager.removeEntitySystem(entityID);
    
    // Remove all components from the entity
    componentManager.removeEntityComponent(entityID);
    
    // Destroy the entity in the entity manager
	std::cout << "[World] destroyEntity(" << entityID << "): about to destroy entity\n";
    entityManager.destroyEntity(entityID);
}

void World::setState(std::unique_ptr<GameState> state)
{
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

void World::setLoadedState(std::unique_ptr<GameState> state, std::string fileName) {
    if (currentState)
    {
        currentState->onExit(*this);
    }
    currentState = std::move(state);
    if (currentState)
    {
        currentState->loadFromFile(*this, fileName);
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
