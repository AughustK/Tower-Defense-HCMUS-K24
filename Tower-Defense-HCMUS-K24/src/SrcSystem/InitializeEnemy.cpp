#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/CircleComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t)
{
    // Spawn enemies off-screen (outside the map)
    float offScreenX = -100.0f; // Spawn enemies to the left of the screen
    float offScreenY = 100.0f;  // Slightly below the top of the screen
    
    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();
		cout << "[EnemySpawnSystem] Spawning enemy " << e << " off-screen at (" << offScreenX << ", " << offScreenY + i * 20 << ")\n";
        
        // Spawn enemies off-screen with slight vertical offset to avoid overlap
        PositionComponent posComp{};
        posComp.x = offScreenX; 
        posComp.y = offScreenY + i * 20; // Stagger enemies vertically
        world.addComponent(e, posComp);

        VelocityComponent vel{};
        vel.x = vel.y = 0.f;
        world.addComponent(e, vel);

        PathComponent pathComp{};
        pathComp.waypoints = path;
        pathComp.currentIndex = 0;
        pathComp.speed = 10.0;
        pathComp.shouldTeleport = true; // Mark for teleportation to first waypoint
        world.addComponent(e, pathComp);

        EnemyComponent enemy;
        
        enemy.type = t;
        enemy.loadStats(); // Set health, speed, etc...
        pathComp.speed = enemy.speed;
        world.addComponent(e, enemy);

        HealthComponent hp{ static_cast<int>(enemy.health) };
        world.addComponent(e, hp);

        float enemyRadius = 20.0f; // Default collision radius for enemies
        CircleComponent circle(posComp.x, posComp.y, enemyRadius, e, CircleComponent::CollisionType::Enemy);
        world.addComponent(e, circle);
        
		std::string spritePath = EnemyComponent::getAnimationPath(enemy.type, map);
		
        float enemyScale = enemy.scale;
        int frameCount = 1;
        float frameRate = 1.0f;
        if (enemy.type == EnemyComponent::EnemyType::FireNormal)
        {
            frameCount = 20;
            frameRate = 0.05f;
        }
        if (enemy.type == EnemyComponent::EnemyType::HellNormal)
        {
            frameCount = 19;
            frameRate = 0.05f;
        }
        if (enemy.type == EnemyComponent::EnemyType::IceNormal)
        {
            frameCount = 13;
            frameRate = 0.08f;
        }
        if (enemy.type == EnemyComponent::EnemyType::ParadiseNormal)
        {
            frameCount = 8;
            frameRate = 0.1f;
        }
        
        if (enemy.type == EnemyComponent::EnemyType::FireBoss)
        {
            frameCount = 4;
            frameRate = 0.2f;
        }

        if (enemy.type == EnemyComponent::EnemyType::HellBoss)
        {
            frameCount = 6;
            frameRate = 0.1f;
        }

        if (enemy.type == EnemyComponent::EnemyType::IceBoss)
        {
            frameCount = 4;
            frameRate = 0.35f;
        }

        if (enemy.type == EnemyComponent::EnemyType::ParadiseBoss)
        {
            frameCount = 2;
            frameRate = 0.6f;
        }

        SpriteComponent spriteE(spritePath, { posComp.x, posComp.y }, { enemyScale, enemyScale });
        spriteE.frameCount = frameCount;
        spriteE.frameRate = frameRate;

        if (spriteE.texture && spriteE.texture->getSize().x > 0)
        {
            int frameWidth = spriteE.texture->getSize().x / frameCount;
            spriteE.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, spriteE.texture->getSize().y));
        }
        // Set origin to center
        sf::FloatRect bounds = spriteE.sprite.getLocalBounds();
        spriteE.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        world.addComponent(e, spriteE);
        createdEnemies.push_back(e);
    }
}

void EnemySpawnSystem::destroyAllEnemies(World& world)
{
    for (EntityID enemy : createdEnemies)
    {
		cout << "[EnemySpawnSystem] Destroying enemy " << enemy << "\n";
        world.destroyEntity(enemy);
    }
	createdEnemies.clear();
}
