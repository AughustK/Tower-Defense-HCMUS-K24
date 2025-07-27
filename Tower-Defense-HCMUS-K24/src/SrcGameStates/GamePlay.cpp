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
#include "../../header/Systems/TowerSystem.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

EntityID moneyTextId = INVALID_ENTITY;

GamePlay::GamePlay(const std::string& mapFilename) : mapFilename(mapFilename)
{
    validTowerSpotsPerMap["FireMap"] = { {510, 330}, {760, 330}, \
    {992, 236}, { 828, 430 }, { 995, 430 }, { 580, 525 }, \
    {788, 630}, { 1027, 630 } };

    validTowerSpotsPerMap["HellMap"] = { {807 , 508}, {1034, 515},\
    {463, 586}, { 410, 753 }, { 800, 748 } };

    validTowerSpotsPerMap["ParadiseMap"] = { {296, 315},{557, 369},\
    {808, 315}, { 482, 544 }, { 778, 461 }, { 1007, 640 }, { 1159, 805 } };

    validTowerSpotsPerMap["IceMap"] = { {328, 448}, {628, 364}, \
    {916, 320}, { 916, 512 }, { 929, 734 }, { 433, 660 }, { 1088, 909 } };
}

std::optional<sf::Vector2f> getValidPlacementSpot(
    const sf::Vector2f& mousePos,
    const std::unordered_map<std::string, std::vector<sf::Vector2f>>& spotData,
    const std::string& mapName,
    float snapRadius)
{
    auto it = spotData.find(mapName);
    if (it == spotData.end()) return std::nullopt;

    const auto& spots = it->second;
    for (const auto& spot : spots) {
        float dx = mousePos.x - spot.x;
        float dy = mousePos.y - spot.y;
        float distSq = dx * dx + dy * dy;
        if (distSq <= snapRadius * snapRadius) {
            std::cout << spot.x << " " << spot.y << endl;
            return spot;
        }
    }
    return std::nullopt;
}

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

    for (size_t i = 0; i < pathWaypoints.size(); ++i)
    {
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

    //money

    moneyTextId = world.createEntity();
    registerEntity(moneyTextId);

    std::string moneyStr = std::to_string(money);
    TextComponent moneyText(moneyStr, 70, fontPath, sf::Color::Yellow, { 95.f, 20.f }, true, sf::Color::Black, 4.f);
    world.addComponent(moneyTextId, moneyText);

    // coin Icon
    EntityID coinIcon = world.createEntity();
    registerEntity(coinIcon);
    const string coinPath = "assets/Icon/Coin.png";
    SpriteComponent coinSprite(coinPath, { 0.f, 5.f }, { 0.28f, 0.28f });
    world.addComponent(coinIcon, coinSprite);

    //tower icon
    spawnTowerIcons(world);


    //button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    const string buttonPath = "assets/Icon/Left/B_Button68.png";
    SpriteComponent spriteComp1(buttonPath, { 0.f, 980.f }, { 5.f, 5.f });
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

    //pause button
    EntityID pauseButton = world.createEntity();
    registerEntity(pauseButton);
    const string pauseButtonPath = "assets/Icon/Pause/A_Pause2.png";
    SpriteComponent pauseSprite(pauseButtonPath, { 1650.f, 975.f }, { 4.35f, 4.35f });
    pauseSprite.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Pause Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<ChooseMap>());
        };
    world.addComponent(pauseButton, soundComp);
    world.addComponent(pauseButton, pauseSprite);

    //setting button
    EntityID settingButton = world.createEntity();
    registerEntity(settingButton);
    const string settingButtonPath = "assets/Icon/Settings/A_Settings2.png";
    SpriteComponent settingSprite(settingButtonPath, { 1800.f, 975.f }, { 4.35f, 4.35f });
    settingSprite.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Setting Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            /*world.setState(std::make_unique<Setting>());*/
        };
    world.addComponent(settingButton, soundComp);
    world.addComponent(settingButton, settingSprite);

    //Notify invalid placement
    EntityID noti = world.createEntity();
    notificationEntity = noti;
    registerEntity(noti);
    const string str = "";
    TextComponent textComp0(str, 50, fontPath, Color(255, 215, 0), { 768, 100 }, false, sf::Color::Black, 7.f);
    world.addComponent(noti, textComp0);

    spawnInitialEntities(world);
    //Ensure our enemy‐spawn timer is reset
    spawnTimer = 0.f;
    currentWave = 0;

    money = 100; // Start with 100 money

    world.getSystem<ProjectilePoolSystem>()->initPool(world, 100);
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

    // Handle left click
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });

        // First, check if any UI element was clicked
        for (EntityID e : entities)
        {
            auto& spriteComp = world.getComponent<SpriteComponent>(e);
            if (spriteComp.tryClick(mousePos, e, world))
            {
                return; // UI was clicked, so don't place a tower
            }
        }

        // Check if tower option buttons were clicked
        auto textEntities = world.getEntitiesWithComponent<TextComponent>();
        bool towerOptionClicked = false;
        for (EntityID e : textEntities)
        {
            auto& textComp = world.getComponent<TextComponent>(e);
            if (textComp.tryClick(mousePos, e, world))
            {
                return; // Tower option button was clicked
                towerOptionClicked = true;
                break;
            }
        }

        // If in placement mode, place the tower
        if (isPlacingTower)
        {
            std::cout << "isPlacingCalled\n";
            auto snappedPosOpt = getValidPlacementSpot(
                mousePos,
                validTowerSpotsPerMap,
                mapFilename,
                50.f
            );

            if (!snappedPosOpt.has_value())
            {
                auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
                notifText.setString("TOWER MUST BE PLACED ON TILES");

                sf::FloatRect bounds = notifText.getLocalBounds();
                notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

                notifText.setPosition(768.f, 100.f);

                notificationActive = true;
                notificationTimer = 0.f;
                isPlacingTower = false;

                return;
            }

            sf::Vector2f snappedPos = *snappedPosOpt;

            // Kiểm tra vị trí đó có bị trùng với trụ khác không
            auto existingTowers = world.getEntitiesWithComponent<TowerComponent>();
            for (EntityID t : existingTowers)
            {
                auto& tc = world.getComponent<TowerComponent>(t);
                if (std::abs(tc.x - snappedPos.x) < 1.0f && std::abs(tc.y - snappedPos.y) < 1.0f) {
                    std::cout << "Already has tower.\n";
                    auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
                    notifText.setString("THIS TILE ALREADY HAS TOWER");

                    sf::FloatRect bounds = notifText.getLocalBounds();
                    notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

                    notifText.setPosition(768.f, 100.f);

                    notificationActive = true;
                    notificationTimer = 0.f;
                    isPlacingTower = false;
                    isPlacingTower = false;
                    return;
                }
            }

            if (money < TowerComponent::getTowerDef(placingType).cost[placingLevel])
            {
                std::cout << "Not enough money to place tower.\n";
                auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
                notifText.setString("NOT ENOUGH MONEY");
                sf::FloatRect bounds = notifText.getLocalBounds();
                notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                notifText.setPosition(768.f, 100.f);
                notificationActive = true;
                notificationTimer = 0.f;
                isPlacingTower = false;
                return;
            }

            // Create the tower entity
            EntityID tower = world.createEntity();
            registerEntity(tower);
			cout << "[GamePlay] Placing tower at position: " << tower << endl;
            TowerComponent towerComp(snappedPos.x, snappedPos.y, placingType, placingLevel);
            world.addComponent(tower, towerComp);

            if ((towerComp.type == TowerComponent::TowerType::Archer && towerComp.level == 1) || (towerComp.type == TowerComponent::TowerType::Mage && towerComp.level == 1))
            {
                towerComp.y -= 12;
                snappedPos.y -= 12;
                towerComp.x += 10;
                snappedPos.x += 10;
            }

            std::string spritePath = TowerComponent::getSpritePath(placingType, placingLevel);
            const TowerDef& def = TowerComponent::getTowerDef(placingType);
            float scale = def.scale[placingLevel];

            SpriteComponent towerSprite(spritePath, snappedPos, { scale, scale });
            sf::FloatRect bounds = towerSprite.sprite.getLocalBounds();
            towerSprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            world.addComponent(tower, towerSprite);

            money -= towerComp.cost;
            isPlacingTower = false;
            std::cout << "Tower Pos: " << snappedPos.x << ", " << snappedPos.y << std::endl;
        }
    }

    // Handle right click for tower interaction
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Right)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });

        // Check if a tower was right-clicked
        EntityID clickedTower = findTowerAtPosition(world, mousePos);
        if (clickedTower != INVALID_ENTITY)
        {
            showTowerOptions(world, clickedTower, mousePos);
        }
        else
        {
            // Hide tower options if clicking elsewhere
            hideTowerOptions(world);
        }
    }

    // Handle mouse movement for hover effects
    if (event.type == sf::Event::MouseMoved)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseMove.x, event.mouseMove.y });

        // Apply hover effects to text components
        auto textEntities = world.getEntitiesWithComponent<TextComponent>();
        for (EntityID e : textEntities)
        {
            auto& textComp = world.getComponent<TextComponent>(e);

            // Custom hover colors for tower option buttons
            if (textComp.txt.getString().find("UPGRADE") != std::string::npos)
            {
                if (textComp.contains(mousePos) && textComp.isHover)
                {
                    textComp.txt.setFillColor(sf::Color::Yellow); // Bright yellow for upgrade hover
                }
                else
                {
                    textComp.txt.setFillColor(textComp.originColor);
                }
            }
            else if (textComp.txt.getString().find("DELETE") != std::string::npos)
            {
                if (textComp.contains(mousePos) && textComp.isHover)
                {
                    textComp.txt.setFillColor(sf::Color(255, 100, 100)); // Light red for delete hover
                }
                else
                {
                    textComp.txt.setFillColor(textComp.originColor);
                }
            }
            else
            {
                // Default hover behavior for other text components
                textComp.tryHover(mousePos, e, world);
            }
        }
    }
}

void GamePlay::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);

    if (notificationActive)
    {
        notificationTimer += dt;
        if (notificationTimer >= 1.f)
        {
            auto& notif = world.getComponent<TextComponent>(notificationEntity);
            notif.txt.setString("");
            notificationActive = false;
        }
    }

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

    auto spriteSys = world.getSystem<SpriteRenderSystem>();
    spriteSys->updateAnimation(dt, world);

    // Synchronize CircleComponent position with PositionComponent
    auto& circleArray = world.getComponentArray<CircleComponent>();
    auto& positionArray = world.getComponentArray<PositionComponent>();
    for (const auto& [entity, _] : circleArray.getEntityToIndexMap()) {
        if (positionArray.containData(entity)) {
            auto& circle = circleArray.getData(entity);
            auto& pos = positionArray.getData(entity);
            circle.x = pos.x;
            circle.y = pos.y;
        }
    }

    //Tower system update
    auto towerSys = world.getSystem<TowerSystem>();
    if (towerSys) {
        towerSys->update(dt, world);
    }

    auto colSys = world.getSystem<CollisionSystem>();
    colSys->update(dt, world);


    // update money
    if (world.hasComponent<TextComponent>(moneyTextId))
    {
        auto& text = world.getComponent<TextComponent>(moneyTextId);
        text.setString(std::to_string(money));
    }
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
    // Hide tower options before exiting
    hideTowerOptions(world);

    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities)
    {
        auto& musicComp = world.getComponent<MusicComponent>(id);
        if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
            musicComp.music->stop();
        }
    }
    auto enemySys = world.getSystem<EnemySpawnSystem>();
    if (enemySys) 
    {
        enemySys->destroyAllEnemies(world);
    }
    for (EntityID id : createdEntities)
    {
        world.destroyEntity(id);
    }
    createdEntities.clear();
    world.getSystem<ProjectilePoolSystem>()->clearPool(world);

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

            string cost = "20";
            EntityID iconEntity = world.createEntity();
            registerEntity(iconEntity);

            TowerIconComponent iconComp;
            iconComp.type = def.type;
            iconComp.level = lv;

            if (def.type == TowerComponent::TowerType::Archer && lv == 1)
            {
                iconScale = 3.0f;
                cost = "50";
            }

            if (def.type == TowerComponent::TowerType::Mage && lv == 0)
            {
                iconScale = 3.9f;
                cost = "30";
            }

            if (def.type == TowerComponent::TowerType::Mage && lv == 1)
            {
                iconScale = 2.6f;
                cost = "70";
            }

            if (def.type == TowerComponent::TowerType::Cannon && lv == 0)
            {
                iconScale = 1.0f;
                cost = "55";
            }

            if (def.type == TowerComponent::TowerType::Cannon && lv == 1)
            {
                iconScale = 1.4f;
                cost = "80";
            }

            // add cost sprite for each tower
            sf::Vector2f posCost{ frameX + paddingX + spacingX * float(lv) + 55, frameY + paddingY + spacingY * float(idx) + 135 };
            EntityID costTextId = world.createEntity();
            registerEntity(costTextId);
            const string fontPath = "assets/Font/Minecraft-Regular.otf";
            TextComponent costText(cost, 28, fontPath, sf::Color::Yellow, posCost, true, sf::Color::Black, 2.5f);
            world.addComponent(costTextId, costText);

            sf::Vector2f posCoin{ frameX + paddingX + spacingX * float(lv) + 7, frameY + paddingY + spacingY * float(idx) + 128 };
            EntityID coinIcon = world.createEntity();
            registerEntity(coinIcon);
            const string coinPath = "assets/Icon/Coin.png";
            SpriteComponent coinSprite(coinPath, posCoin, { 0.12f, 0.12f });
            world.addComponent(coinIcon, coinSprite);


            // position for the tower icon
            sf::Vector2f pos{ frameX + paddingX + spacingX * float(lv), frameY + paddingY + spacingY * float(idx) };

            string iconPath = TowerComponent::getSpritePath(def.type, lv);
            iconComp.sprite = SpriteComponent(iconPath, pos, { iconScale, iconScale });
            iconComp.sprite.onClick = [this, def, lv](EntityID entityId, World& world)
                {
                    this->isPlacingTower = true;
                    this->placingType = def.type;
                    this->placingLevel = lv;
                    std::cout << "Selected tower: " << int(def.type) << " level: " << lv << std::endl;
                };
            world.addComponent(iconEntity, iconComp);
            world.addComponent(iconEntity, iconComp.sprite); // This sprite has the onClick handler!

        }
        ++idx;
    }


}

EntityID GamePlay::findTowerAtPosition(World& world, const sf::Vector2f& mousePos)
{
    auto towers = world.getEntitiesWithComponent<TowerComponent>();
    for (EntityID towerId : towers)
    {
        if (world.hasComponent<SpriteComponent>(towerId))
        {
            auto& spriteComp = world.getComponent<SpriteComponent>(towerId);
            if (spriteComp.contains(mousePos))
            {
                return towerId;
            }
        }
    }
    return INVALID_ENTITY;
}

void GamePlay::showTowerOptions(World& world, EntityID towerId, const sf::Vector2f& mousePos)
{
    // Hide any existing options first
    hideTowerOptions(world);

    selectedTower = towerId;
    towerOptionsVisible = true;
    towerOptionEntities.clear(); // Clear the tracking vector

    const string fontPath = "assets/Font/Minecraft-Regular.otf";

    // Check if tower can be upgraded
    auto& towerComp = world.getComponent<TowerComponent>(towerId);
    bool canUpgrade = (towerComp.level < 1);
    int upgradeCost = canUpgrade ? TowerComponent::getTowerDef(towerComp.type).cost[1] : 0;

 
    upgradeButton = world.createEntity();
    //registerEntity(upgradeButton);
    towerOptionEntities.push_back(upgradeButton);

    std::string upgradeText = canUpgrade ? "UPGRADE\n" + std::to_string(upgradeCost) + "G " : "MAX\nLEVEL";
    sf::Color upgradeColor = canUpgrade ? sf::Color::Green : sf::Color::Red;

    TextComponent upgradeTextComp(upgradeText, 24, fontPath, upgradeColor,
        { mousePos.x - 80, mousePos.y - 50 }, true, sf::Color::Black, 2.0f);

    // Set custom hover color for upgrade button
    if (canUpgrade)
    {
        upgradeTextComp.originColor = sf::Color::Green;
    }
    else
    {
        upgradeTextComp.originColor = sf::Color::Red;
    }

    if (canUpgrade)
    {
        upgradeTextComp.onClick = [this, towerId](EntityID entityId, World& world) {
            this->upgradeTower(world, towerId);
            };
    }

    world.addComponent(upgradeButton, upgradeTextComp);

    // Create delete text button (no background sprite)
    deleteButton = world.createEntity();
    //registerEntity(deleteButton);
    towerOptionEntities.push_back(deleteButton);

    TextComponent deleteTextComp("SELL", 24, fontPath, sf::Color::Red,
        { mousePos.x + 80, mousePos.y - 50 }, true, sf::Color::Black, 2.0f);

    // Set custom hover color for delete button
    deleteTextComp.originColor = sf::Color::Red;

    deleteTextComp.onClick = [this, towerId](EntityID entityId, World& world) {
        this->deleteTower(world, towerId);
        };

    world.addComponent(deleteButton, deleteTextComp);

    // Add coin icon for upgrade cost
    if (canUpgrade)
    {
        EntityID coinIconId = world.createEntity();
        //registerEntity(coinIconId);
        towerOptionEntities.push_back(coinIconId);

        const string coinPath = "assets/Icon/Coin.png";
        sf::Vector2f coinPos = { mousePos.x - 90, mousePos.y - 40 };
        SpriteComponent coinSprite(coinPath, coinPos, { 0.15f, 0.1f });

        if (canUpgrade)
        {
            coinSprite.onClick = [this, towerId](EntityID entityId, World& world) {
                this->upgradeTower(world, towerId);
                };
        }

        world.addComponent(coinIconId, coinSprite);
    }

    // Add selection highlight around the tower
    EntityID highlightId = world.createEntity();
    //registerEntity(highlightId);
    towerOptionEntities.push_back(highlightId);

    // Use a simple highlight by scaling the tower sprite slightly and changing its color
    // We'll create a larger, semi-transparent version of the tower sprite
    if (world.hasComponent<SpriteComponent>(towerId))
    {
        auto& originalSprite = world.getComponent<SpriteComponent>(towerId);
        SpriteComponent highlightSprite = originalSprite; // Copy the original sprite

        highlightSprite.sprite.setColor(sf::Color(255, 255, 0, 150)); // Semi-transparent yellow

        world.addComponent(highlightId, highlightSprite);
    }
}

void GamePlay::hideTowerOptions(World& world)
{
    if (towerOptionsVisible)
    {
        // Destroy all tracked tower option entities
        for (EntityID e : towerOptionEntities)
        {
            world.destroyEntity(e);
        }

        towerOptionEntities.clear();
        upgradeButton = INVALID_ENTITY;
        deleteButton = INVALID_ENTITY;
        selectedTower = INVALID_ENTITY;
        towerOptionsVisible = false;
    }
}

void GamePlay::upgradeTower(World& world, EntityID towerId)
{
    auto& towerComp = world.getComponent<TowerComponent>(towerId);

    // Check if tower can be upgraded
    if (towerComp.level >= 1)
    {
        auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
        notifText.setString("TOWER IS ALREADY MAX LEVEL");
        sf::FloatRect bounds = notifText.getLocalBounds();
        notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        notifText.setPosition(768.f, 100.f);
        notificationActive = true;
        notificationTimer = 0.f;
        return;
    }

    // Check if player has enough money
    int upgradeCost = TowerComponent::getTowerDef(towerComp.type).cost[1];
    if (money < upgradeCost)
    {
        auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
        notifText.setString("NOT ENOUGH MONEY TO UPGRADE");
        sf::FloatRect bounds = notifText.getLocalBounds();
        notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        notifText.setPosition(768.f, 100.f);
        notificationActive = true;
        notificationTimer = 0.f;
        return;
    }

    // Upgrade the tower
    towerComp.upgrade();
    money -= upgradeCost;

    // Play upgrade sound
    const string upgradeSoundPath = "assets/SFX/MouseClick.mp3";
    SoundComponent upgradeSound(upgradeSoundPath, false);
    upgradeSound.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    upgradeSound.sound->play();

    // Update the tower sprite
    if (world.hasComponent<SpriteComponent>(towerId))
    {
        world.removeComponent<SpriteComponent>(towerId);
    }

    // Create new sprite component for upgraded tower
    std::string newSpritePath = TowerComponent::getSpritePath(towerComp.type, towerComp.level);
    const TowerDef& def = TowerComponent::getTowerDef(towerComp.type);
    float newScale = def.scale[towerComp.level];

    // Handle position adjustments for specific tower types
    sf::Vector2f spritePos = { towerComp.x, towerComp.y };
    if ((towerComp.type == TowerComponent::TowerType::Archer && towerComp.level == 1) ||
        (towerComp.type == TowerComponent::TowerType::Mage && towerComp.level == 1))
    {
        spritePos.y -= 12;
        spritePos.x += 10;
    }

    SpriteComponent newSpriteComp(newSpritePath, spritePos, { newScale, newScale });
    sf::FloatRect twBounds = newSpriteComp.sprite.getLocalBounds();
    newSpriteComp.sprite.setOrigin(twBounds.width / 2.f, twBounds.height / 2.f);
    world.addComponent(towerId, newSpriteComp);


    // Show success notification
    auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
    notifText.setString("TOWER UPGRADED!");
    sf::FloatRect bounds = notifText.getLocalBounds();
    notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    notifText.setPosition(768.f, 100.f);
    notificationActive = true;
    notificationTimer = 0.f;

    // Hide the options
    hideTowerOptions(world);
}

void GamePlay::deleteTower(World& world, EntityID towerId)
{
    // Get tower cost for refund (half of the total cost)
    auto& towerComp = world.getComponent<TowerComponent>(towerId);
    int totalCost = 0;

    // Calculate total cost (base + upgrade if applicable)
    totalCost += TowerComponent::getTowerDef(towerComp.type).cost[0]; // Base cost
    if (towerComp.level > 0)
    {
        totalCost += TowerComponent::getTowerDef(towerComp.type).cost[1]; // Upgrade cost
    }

    // Refund half the cost
    int refund = totalCost / 2;
    money += refund;

    // Play delete sound
    const string deleteSoundPath = "assets/SFX/MouseClick.mp3";
    SoundComponent deleteSound(deleteSoundPath, false);
    deleteSound.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    deleteSound.sound->play();

    // Destroy the tower
    world.destroyEntity(towerId);

    // Show success notification
    auto& notifText = world.getComponent<TextComponent>(notificationEntity).txt;
    notifText.setString("TOWER SOLD FOR " + std::to_string(refund) + " GOLD!");
    sf::FloatRect bounds = notifText.getLocalBounds();
    notifText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    notifText.setPosition(768.f, 100.f);
    notificationActive = true;
    notificationTimer = 0.f;

    // Hide the options
    hideTowerOptions(world);
}

