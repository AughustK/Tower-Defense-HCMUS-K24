#include "../../header/GameStates/GamePlay.h"
#include "../../header/Managers/World.h"
#include "../../header/Managers/MapLoad.h"
#include "../../header/GameStates/ChooseMap.h"

#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/PathfindingComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/UITextComponent.h"
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/ClickComponent.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ShopComponent.h"
#include "../../header/Components/TowerComponent.h"
#include "../../header/Components/TowerDef.h"


#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Systems/PhysicSystem.h"
#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Systems/InitializeProjectile.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Systems/TextRenderSystem.h"

#include <fstream>
#include <sstream>
#include <iostream>

GamePlay::GamePlay(const std::string& mapFilename) : mapFilename(mapFilename) {}


void GamePlay::onEnter(World& world)
{
    std::cout << "[Gameplay] onEnter called\n";

    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities)
    {
        world.destroyEntity(id);
    }

    std::string fullPath = "assets/" + mapFilename + "/" + mapFilename + ".txt";
    MapLoader::loadFromFile(fullPath, pathWaypoints);
    bool ok = MapLoader::loadFromFile(fullPath, pathWaypoints);

    for (size_t i = 0; i < pathWaypoints.size(); ++i) {
        const auto& p = pathWaypoints[i];
        std::cout << "[GamePlay] waypoint[" << i << "] = ("
            << p.x << ", " << p.y << ")\n";
    }

    

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Map/" + mapFilename + ".mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    MusicComponent musicComp(musicPath);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/" + mapFilename + "/" + mapFilename + ".jpg";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);
    world.addComponent(background, musicComp);


    //tower frame
	EntityID towerFrame = world.createEntity();
	registerEntity(towerFrame);
    const string towerFramePath = "assets/Bg/ShopFrame.png";
	cout << "[GamePlay] Tower frame entity ID: " << towerFrame << endl;
	SpriteComponent towerFrameSprite(towerFramePath, { 1613.5f, 0.f }, { 0.5, 1.18f });
	world.addComponent(towerFrame, towerFrameSprite);


    //frame text
    const string fontPath = "assets/Font/Minecraft-Regular.otf";
	EntityID towerHeader = world.createEntity();
	registerEntity(towerHeader);
	const string headerText = "Heroes Shop";
	TextComponent towerHeaderText(headerText, 32, fontPath, sf::Color::White, { 1750.f, 120.f }, true, sf::Color::Black, 5.f);
	world.addComponent(towerHeader, towerHeaderText);

    //tower icon
    spawnTowerIcons(world);
    

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


    spawnInitialEntities(world);
    //Ensure our enemy‐spawn timer is reset
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
            if (spriteComp.tryClick(mousePos, e, world))
            {
                return;
            }
        }
    }
}

void GamePlay::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);

    //Spawn next wave?
    spawnTimer += dt;
    if (enemiesToSpawn == 0 && currentWave < static_cast<int>(waveSizes.size()) && spawnTimer >= waveInterval)
    {
        spawnWave(world);
		cout << "[GamePlay] Spawning wave " << currentWave << " with " << waveSizes[currentWave] << " enemies.\n";
        
    }

	//Spawn enemies for the current wave
    if (enemiesToSpawn > 0) 
    {
        enemySpawnTimer += dt;
        if (enemySpawnTimer >= enemySpawnInterval) 
        {
            EnemyComponent::EnemyType typeToSpawn = EnemyComponent::EnemyType::Normal;
            if (currentWave == static_cast<int>(waveSizes.size()) - 1)
            {
                typeToSpawn = EnemyComponent::EnemyType::Boss;
            }
            world.getSystem<EnemySpawnSystem>()->spawnWave(world, currentWavePath, 1, mapFilename, typeToSpawn);
            enemiesToSpawn--;
            enemySpawnTimer = 0.0f;
            if (enemiesToSpawn == 0) 
            {
                currentWave++; // Only increment after all enemies for this wave are spawned
            }
        }
        spawnTimer = 0.f;
    }


    auto pathSys = world.getSystem<PathFollowingSystem>();
    pathSys->update(dt, world);

    auto phySys = world.getSystem<PhysicSystem>();
    phySys->update(dt, world);

    auto colSys = world.getSystem<CollisionSystem>();
    colSys->update(dt, world);
}

void GamePlay::spawnWave(World& world)
{
    enemiesToSpawn = waveSizes[currentWave];
    enemySpawnTimer = 0.0f;
    currentWavePath = pathWaypoints; 
}


void GamePlay::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);

    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);

    const float radius = 5.f;
    sf::CircleShape debugDot(radius);
    debugDot.setOrigin(radius, radius);
    debugDot.setFillColor(sf::Color::Green);

    for (auto& wp : pathWaypoints) 
    {
        debugDot.setPosition(wp.x, wp.y);
        window.draw(debugDot);
    }
}

void GamePlay::onExit(World& world) {
    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities)
    {
        auto& musicComp = world.getComponent<MusicComponent>(id);
        if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
            musicComp.music->stop();
        }
    }
	auto enemySys = world.getSystem<EnemySpawnSystem>();
    if (enemySys) {
        enemySys->destroyAllEnemies(world);
	}
    for (EntityID id : createdEntities)
    {
        cout << id << endl;
        world.destroyEntity(id);
    }
    createdEntities.clear();
    cout << "[Gameplay] Exit state and free memory successfully.\n";
}

void GamePlay::spawnTowerIcons(World& world)
{
    // position and layout parameters
    const float frameX = 1635.f;
    const float frameY = 200.f;
    const float paddingX = 25.0f;   // inset from shop‐frame border
    const float paddingY = 30.0f;
    const float spacingX = 100.0f;   // distance between icons
	const float spacingY = 200.0f;  // distance between levels
    float iconScale = 4.8f;

    size_t idx = 0;
    for (const TowerDef& def : TOWER_DEFS)
    {
        for (int lv = 0; lv < 2; ++lv)
        {

            EntityID iconEntity = world.createEntity();
            registerEntity(iconEntity);

            TowerIconComponent iconComp;
            iconComp.type = def.type;
            iconComp.level = lv;

            if(def.type == TowerComponent::TowerType::Archer && lv == 1)
            {
                iconScale = 3.0f;
			}

            if (def.type == TowerComponent::TowerType::Mage && lv == 0)
            {
                iconScale = 3.9f;
            }

            if (def.type == TowerComponent::TowerType::Mage && lv == 1)
            {
                iconScale = 2.6f;
			}

            if (def.type == TowerComponent::TowerType::Cannon && lv == 0)
            {
                iconScale = 1.0f;
            }

            if (def.type == TowerComponent::TowerType::Cannon && lv == 1)
            {
                iconScale = 1.4f;
            }

            sf::Vector2f pos{ frameX + paddingX + spacingX * float(lv), frameY + paddingY + spacingY * float(idx) };

            string iconPath = TowerComponent::getSpritePath(def.type, lv);
            iconComp.sprite = SpriteComponent(iconPath, pos, { iconScale, iconScale });
            world.addComponent(iconEntity, iconComp);
            world.addComponent(iconEntity,
                SpriteComponent(iconPath, pos, { iconScale,iconScale })
            );
        }
        ++idx;
    }

    
}

//void GamePlay::spawnProjectile(float x, float y, const sf::Vector2f& velocity, ProjectileComponent::ProjectileType type, World& world)
//{
//    auto pool = world.getSystem<ProjectilePoolSystem>();
//    
//}

