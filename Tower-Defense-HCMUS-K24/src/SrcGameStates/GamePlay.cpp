#include "../../header/GameStates/GamePlay.h"
#include "../../header/Managers/World.h"
#include "../../header/Managers/MapLoad.h"
#include "../../header/GameStates/ChooseMap.h"

#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/PathfindingComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/ClickComponent.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"

#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Systems/PhysicSystem.h"
#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"

#include <fstream>
#include <sstream>
#include <iostream>

GamePlay::GamePlay(const std::string& mapFilename) : mapFilename(mapFilename) {}


void GamePlay::onEnter(World& world)
{
    std::cout << "[Gameplay] onEnter called\n";

    std::string fullPath = "assets/" + mapFilename + "/" + mapFilename + ".txt";
    MapLoader::loadFromFile(fullPath, pathWaypoints);
    bool ok = MapLoader::loadFromFile(fullPath, pathWaypoints);

    for (size_t i = 0; i < pathWaypoints.size(); ++i) {
        const auto& p = pathWaypoints[i];
        std::cout << "[GamePlay] waypoint[" << i << "] = ("
            << p.x << ", " << p.y << ")\n";
    }

    spawnInitialEntities(world);

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Map/" + mapFilename +".mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    MusicComponent musicComp(musicPath);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/" + mapFilename + "/" + mapFilename + ".png";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);
    world.addComponent(background, musicComp);

    //button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    const string buttonPath = "assets/Icon/Left/B_Button68.png";
    SpriteComponent spriteComp1(buttonPath, { 0.f, 0.f }, { 5.f, 5.f });
    spriteComp1.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<ChooseMap>());
        };
    world.addComponent(exitButton, soundComp);
    world.addComponent(exitButton, spriteComp1);


    // Ensure our enemy‐spawn timer is reset
    spawnTimer = 0.f;
    currentWave = 0;
}

void GamePlay::spawnInitialEntities(World& world)
{
    auto e = world.createEntity();

    PositionComponent pos{ pathWaypoints.front().x, pathWaypoints.front().y, PositionComponent::Type::Enemy };
    world.addComponent(e, pos);

    VelocityComponent vel{ 0.f, 0.f };
    world.addComponent(e, vel);


    PathComponent pc;
    pc.waypoints = pathWaypoints;
    pc.currentIndex = 0;
    pc.speed = 80.f;  // pixels/sec
    world.addComponent(e, pc);

    
    HealthComponent hp{ 1 };
    world.addComponent(e, hp);

    // 5) (Optional) Add a sprite so you can see it:
    SpriteComponent sprite{};
    world.addComponent(e, sprite);

    createdEntities.push_back(e);
}

void GamePlay::handleEvent(World& world, sf::Event& event)
{
    auto entities = world.getEntitiesWithComponent<SpriteComponent>();
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });
        for (EntityID e : entities)
        {
            auto& spriteComp = world.getComponent<SpriteComponent>(e);
            spriteComp.tryClick(mousePos, e, world);
        }
    }
    // TODO: handle UI clicks / tower placement here
}

void GamePlay::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);

    //Spawn next wave?
    spawnTimer += dt;
    if (currentWave < static_cast<int>(waveSizes.size()) && spawnTimer >= waveInterval)
    {
        spawnWave(world);
        ++currentWave;
        spawnTimer = 0.f;
    }

    auto pathSys = world.getSystem<PathFollowingSystem>();
    pathSys->update(dt, world);

    auto phySys = world.getSystem<PhysicSystem>();
    phySys->update(dt, world);

    auto colSys = world.getSystem<CollisionSystem>();
    colSys->update(dt, world);

    auto spriteSys = world.getSystem<SpriteRenderSystem>();
    spriteSys->updateAnimation(dt, world);
}

void GamePlay::spawnWave(World& world)
{
    int count = waveSizes[currentWave];
    world.getSystem<EnemySpawnSystem>()->spawnWave(world, pathWaypoints, count);

}

void GamePlay::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);

    const float radius = 5.f;
    sf::CircleShape debugDot(radius);
    debugDot.setOrigin(radius, radius);
    debugDot.setFillColor(sf::Color::Green);

    for (auto& wp : pathWaypoints) {
        debugDot.setPosition(wp.x, wp.y);
        window.draw(debugDot);
    }
}

