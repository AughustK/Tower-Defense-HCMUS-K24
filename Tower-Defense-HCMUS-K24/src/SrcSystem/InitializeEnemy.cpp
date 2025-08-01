#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/CircleComponent.h"
#include "../../header/Components/EnemyHPComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::initPool(World& world, std::size_t count)
{
    enemyPool.clear();
    enemyPool.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        EntityID e = world.createEntity();
        world.addComponent(e, PositionComponent());
        world.addComponent(e, VelocityComponent());
        world.addComponent(e, PathComponent());
        world.addComponent(e, EnemyComponent());
        world.addComponent(e, HealthComponent());
        world.addComponent(e, CircleComponent());
        world.addComponent(e, SpriteComponent());
        world.addComponent(e, EnemyHPComponent());
        enemyPool.push_back(e);
    }
    nextPoolIndex = 0;
}

EntityID EnemySpawnSystem::spawnFromPool(World& world, const std::vector<sf::Vector2f>& path, const string& map, 
                                        EnemyComponent::EnemyType t, DifficultyLevel difficulty)
{
    // Get entity from pool
    EntityID e = enemyPool[nextPoolIndex];
    nextPoolIndex = (nextPoolIndex + 1) % enemyPool.size();

    // Spawn enemies off-screen
    float offScreenX = -100.0f;
    float offScreenY = 100.0f;
    
    // Position
    auto& posComp = world.getComponent<PositionComponent>(e);
    posComp.x = offScreenX;
    posComp.y = offScreenY;

    // Velocity
    auto& vel = world.getComponent<VelocityComponent>(e);
    vel.x = vel.y = 0.f;

    // Path
    auto& pathComp = world.getComponent<PathComponent>(e);
    pathComp.waypoints = path;
    pathComp.currentIndex = 0;
    pathComp.speed = 10.0;
    pathComp.shouldTeleport = true;

    // Enemy
    auto& enemy = world.getComponent<EnemyComponent>(e);
    enemy.type = t;
    enemy.difficulty = difficulty;
    enemy.loadStats(difficulty);
    pathComp.speed = enemy.speed;
    cout << "[EnemySpawnSystem] Spawning enemy of type: " << static_cast<int>(enemy.type) 
		<< " with difficulty: " << static_cast<int>(difficulty) << endl;
	cout << "[EnemySpawnSystem] Enemy stats - Health: " << enemy.health
		<< ", Speed: " << enemy.speed
		<< ", Damage: " << enemy.damage
		<< ", Scale: " << enemy.scale
		<< ", Prize: " << enemy.prize << endl;

    // Health
    auto& hp = world.getComponent<HealthComponent>(e);
    hp.currentHealth = static_cast<int>(enemy.health);
    hp.maxHealth = static_cast<int>(enemy.health);

    // Circle
    float enemyRadius = 20.0f;
    auto& circle = world.getComponent<CircleComponent>(e);
    circle.x = posComp.x;
    circle.y = posComp.y;
    circle.radius = enemyRadius;
    circle.tag = CircleComponent::CollisionType::Enemy;

    // Sprite
    std::string spritePath = EnemyComponent::getAnimationPath(enemy.type, map);
    auto& spriteE = world.getComponent<SpriteComponent>(e);
    spriteE.setTxt(spritePath);
    spriteE.sprite.setPosition(posComp.x, posComp.y);
    
    float enemyScale = enemy.scale;
    int frameCount = 1;
    float frameRate = 1.0f;
    
    // Set animation parameters based on enemy type
    if (enemy.type == EnemyComponent::EnemyType::FireNormal1) {
        frameCount = 20;
        frameRate = 0.05f;
    }
    if (enemy.type == EnemyComponent::EnemyType::FireNormal2) {
        frameCount = 6;
        frameRate = 0.2f;
    }
    if (enemy.type == EnemyComponent::EnemyType::HellNormal1) {
        frameCount = 19;
        frameRate = 0.05f;
    }
    if (enemy.type == EnemyComponent::EnemyType::HellNormal2) {
        frameCount = 6;
        frameRate = 0.2f;
    }
    if (enemy.type == EnemyComponent::EnemyType::IceNormal1) {
        frameCount = 13;
        frameRate = 0.08f;
    }
    if (enemy.type == EnemyComponent::EnemyType::IceNormal2) {
        frameCount = 10;
        frameRate = 0.1f;
    }
    if (enemy.type == EnemyComponent::EnemyType::ParadiseNormal1) {
        frameCount = 8;
        frameRate = 0.1f;
    }
    if (enemy.type == EnemyComponent::EnemyType::ParadiseNormal2) {
        frameCount = 6;
        frameRate = 0.2f;
    }
    if (enemy.type == EnemyComponent::EnemyType::FireBoss) {
        frameCount = 4;
        frameRate = 0.2f;
    }
    if (enemy.type == EnemyComponent::EnemyType::HellBoss) {
        frameCount = 6;
        frameRate = 0.1f;
    }
    if (enemy.type == EnemyComponent::EnemyType::IceBoss) {
        frameCount = 4;
        frameRate = 0.2f;
    }
    if (enemy.type == EnemyComponent::EnemyType::ParadiseBoss) {
        frameCount = 2;
        frameRate = 0.6f;
    }

    spriteE.frameCount = frameCount;
    spriteE.frameRate = frameRate;
    spriteE.sprite.setScale(enemyScale, enemyScale);

    if (spriteE.texture && spriteE.texture->getSize().x > 0) {
        int frameWidth = spriteE.texture->getSize().x / frameCount;
        spriteE.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, spriteE.texture->getSize().y));
    }
    
    // Set origin to center
    sf::FloatRect bounds = spriteE.sprite.getLocalBounds();
    spriteE.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    if (enemy.type == EnemyComponent::EnemyType::HellBoss) {
        spriteE.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f + 15.f);
    }

    // Health bar
    auto& healthBar = world.getComponent<EnemyHPComponent>(e);
    healthBar.init(hp.maxHealth, hp.currentHealth, 40.f, 5.f, sf::Vector2f(-30.f, -50.f));

    createdEnemies.push_back(e);
    return e;
}

void EnemySpawnSystem::returnToPool(World& world, EntityID enemyID)
{
    // Move enemy off-screen and reset its state
    if (world.hasComponent<PositionComponent>(enemyID)) {
        auto& pos = world.getComponent<PositionComponent>(enemyID);
        pos.x = -1500;
        pos.y = -1500;
    }

    if (world.hasComponent<VelocityComponent>(enemyID)) {
        auto& vel = world.getComponent<VelocityComponent>(enemyID);
        vel.x = 0.0f;
        vel.y = 0.0f;
    }

    if (world.hasComponent<PathComponent>(enemyID)) {
        auto& path = world.getComponent<PathComponent>(enemyID);
        path.waypoints.clear();
        path.currentIndex = 0;
        path.speed = 0.0f;
        path.shouldTeleport = false;
    }

    if (world.hasComponent<EnemyComponent>(enemyID)) {
        auto& enemy = world.getComponent<EnemyComponent>(enemyID);
        enemy.health = 0;
        enemy.speed = 0;
        enemy.damage = 0;
    }

    if (world.hasComponent<HealthComponent>(enemyID)) {
        auto& hp = world.getComponent<HealthComponent>(enemyID);
        hp.currentHealth = 0;
        hp.maxHealth = 0;
    }

    if (world.hasComponent<CircleComponent>(enemyID)) {
        auto& circle = world.getComponent<CircleComponent>(enemyID);
        circle.x = -1500;
        circle.y = -1500;
    }

    if (world.hasComponent<SpriteComponent>(enemyID)) {
        auto& sprite = world.getComponent<SpriteComponent>(enemyID);
        sprite.sprite.setPosition(-1500, -1500);
    }

    // Remove from created enemies list
    auto it = std::find(createdEnemies.begin(), createdEnemies.end(), enemyID);
    if (it != createdEnemies.end()) {
        createdEnemies.erase(it);
    }
}

void EnemySpawnSystem::clearPool(World& world)
{
    for (EntityID enemyID : enemyPool) {
        std::cout << "[EnemySpawnSystem] Destroying enemy " << enemyID << std::endl;
        world.destroyEntity(enemyID);
    }
    enemyPool.clear();
    createdEnemies.clear();
    nextPoolIndex = 0;
}

// Legacy spawnWave method (now uses pool)
void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t, DifficultyLevel difficulty)
{
    for (std::size_t i = 0; i < count; ++i) {
        spawnFromPool(world, path, map, t, difficulty);
    }
}

void EnemySpawnSystem::destroyAllEnemies(World& world)
{
    for (EntityID enemy : createdEnemies) {
        std::cout << "[EnemySpawnSystem] Destroying enemy " << enemy << std::endl;
        world.destroyEntity(enemy);
    }
    createdEnemies.clear();
}



