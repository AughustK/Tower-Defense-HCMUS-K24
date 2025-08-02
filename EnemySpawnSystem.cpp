#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/CircleComponent.h"
#include "../../header/Components/ActiveComponent.h"

void EnemySpawnSystem::update(float deltaTime)
{
    // Update all active enemies
    for (EntityID enemyID : createdEnemies) {
        if (!world.getComponent<ActiveComponent>(enemyID).active) {
            continue;
        }

        // Update sprite position to match entity position
        if (world.hasComponent<PositionComponent>(enemyID) && world.hasComponent<SpriteComponent>(enemyID)) {
            auto& pos = world.getComponent<PositionComponent>(enemyID);
            auto& sprite = world.getComponent<SpriteComponent>(enemyID);
            sprite.sprite.setPosition(pos.x, pos.y);
        }

        // Update sprite animation
        if (world.hasComponent<SpriteComponent>(enemyID)) {
            auto& spriteComp = world.getComponent<SpriteComponent>(enemyID);
            
            // Update animation timer
            spriteComp.animationTimer += deltaTime;
            
            // Check if it's time to advance to next frame
            if (spriteComp.animationTimer >= spriteComp.frameRate) {
                spriteComp.animationTimer = 0.0f;
                spriteComp.currentFrame = (spriteComp.currentFrame + 1) % spriteComp.frameCount;
                
                // Update texture rectangle for current frame
                if (spriteComp.texture && spriteComp.texture->getSize().x > 0) {
                    int frameWidth = spriteComp.texture->getSize().x / spriteComp.frameCount;
                    int frameHeight = spriteComp.texture->getSize().y;
                    spriteComp.sprite.setTextureRect(sf::IntRect(
                        spriteComp.currentFrame * frameWidth, 0, 
                        frameWidth, frameHeight
                    ));
                }
            }
        }

        // Update circle component position to match sprite
        if (world.hasComponent<PositionComponent>(enemyID) && world.hasComponent<CircleComponent>(enemyID)) {
            auto& pos = world.getComponent<PositionComponent>(enemyID);
            auto& circle = world.getComponent<CircleComponent>(enemyID);
            circle.x = pos.x;
            circle.y = pos.y;
        }
    }
}

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
		world.addComponent(e, ActiveComponent()); 
        enemyPool.push_back(e);
    }
    nextPoolIndex = 0;
}

EntityID EnemySpawnSystem::spawnFromPool(World& world, const std::vector<sf::Vector2f>& path, const string& map, 
                                        EnemyComponent::EnemyType t, DifficultyLevel difficulty)
{
    // Get entity from pool
    EntityID e = enemyPool[nextPoolIndex];
    nextPoolIndex = nextPoolIndex + 1;
    cout << "[EnemySpawnSystem] Spawning enemy" << e << "\n";

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

    // Health
    auto& hp = world.getComponent<HealthComponent>(e);
    hp.currentHealth = static_cast<int>(enemy.health);
    hp.maxHealth = static_cast<int>(enemy.health);

    // Circle
    float enemyRadius = 18.0f;
    auto& circle = world.getComponent<CircleComponent>(e);
    circle.x = posComp.x;
    circle.y = posComp.y;
    circle.radius = enemyRadius;
    circle.tag = CircleComponent::CollisionType::Enemy;

    // Sprite
    std::string spritePath = EnemyComponent::getAnimationPath(enemy.type, map);
    auto& spriteE = world.getComponent<SpriteComponent>(e);
    spriteE.setTxt(spritePath);
    
    // Check if texture loaded successfully
    if (!spriteE.texture) {
        cout << "[EnemySpawnSystem] ERROR: Failed to load texture: " << spritePath << "\n";
        return e; // Return early if texture failed to load
    }
    
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
    spriteE.currentFrame = 0; // Initialize current frame
    spriteE.animationTimer = 0.0f; // Initialize animation timer
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

    // Activate the entity
    world.getComponent<ActiveComponent>(e).active = true;
    createdEnemies.push_back(e);
    return e;
}

void EnemySpawnSystem::returnToPool(World& world, EntityID enemyID)
{
	// Unactivate the enemy entity
    if (world.hasComponent<ActiveComponent>(enemyID))
    {
        world.getComponent<ActiveComponent>(enemyID).active = false;
    }

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

    // Reset sprite animation state
    if (world.hasComponent<SpriteComponent>(enemyID)) {
        auto& sprite = world.getComponent<SpriteComponent>(enemyID);
        sprite.currentFrame = 0;
        sprite.animationTimer = 0.0f;
    }

    // Clean up animation data for hidden enemies
    world.getSystem<SpriteRenderSystem>()->hideEntity(enemyID);

    // Remove from alive enemies list
    auto it = std::find(createdEnemies.begin(), createdEnemies.end(), enemyID);
    if (it != createdEnemies.end()) {
        createdEnemies.erase(it);
    }
}

void EnemySpawnSystem::clearPool(World& world)
{
    // Return all created enemies to pool instead of destroying them
    for (EntityID enemyID : createdEnemies) 
    {
        returnToPool(world, enemyID);
    }
    createdEnemies.clear();
    
    // Reset pool index for reuse
    nextPoolIndex = 0;
}

int EnemySpawnSystem::getRemainEnemy() const
{
	return static_cast<int>(createdEnemies.size());
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
    // Return all enemies to pool instead of destroying them
    for (EntityID enemy : createdEnemies) {
        std::cout << "[EnemySpawnSystem] Returning enemy " << enemy << " to pool" << std::endl;
        returnToPool(world, enemy);
    }
    createdEnemies.clear();
}