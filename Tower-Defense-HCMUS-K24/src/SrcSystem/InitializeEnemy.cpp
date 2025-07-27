#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"
#include "../../header/Components/CircleComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();
		cout << "[EnemySpawnSystem] Spawning enemy " << e << "\n";
        PositionComponent posComp{};
        posComp.x = path.front().x + i; 
        posComp.y = path.front().y;
        world.addComponent(e, posComp);

        VelocityComponent vel{};
        vel.x = vel.y = 0.f;
        world.addComponent(e, vel);

        PathComponent pathComp{};
        pathComp.waypoints = path;
        pathComp.currentIndex = 0;
        pathComp.speed = 10.0;
        world.addComponent(e, pathComp);

        EnemyComponent enemy;
        
        enemy.type = t;
        enemy.loadStats(); // Set health, speed, etc...
        pathComp.speed = enemy.speed;
        world.addComponent(e, enemy);

        cout << "Enemy stat: " << "Type: " << static_cast<int>(enemy.type)
             << ", Health: " << enemy.health
             << ", Speed: " << enemy.speed
             << ", Damage: " << enemy.damage
			<< "\n";

        HealthComponent hp{ static_cast<int>(enemy.health) };
        world.addComponent(e, hp);

        float enemyRadius = 20.0f; // Default collision radius for enemies
        CircleComponent circle(posComp.x, posComp.y, enemyRadius, e, CircleComponent::CollisionType::Enemy);
        world.addComponent(e, circle);
        
		std::string spritePath = EnemyComponent::getAnimationPath(enemy.type, map);
		
        float enemyScale = 0.2f;
        int frameCount = 1;
        float frameRate = 1.0f;
        if (enemy.type == EnemyComponent::EnemyType::Normal) {
            if (map == "FireMap") {
                frameCount = 20;
                frameRate = 0.05f;
            }
            if (map == "HellMap") {
                frameCount = 20;
                frameRate = 0.05f;
            }
            if (map == "IceMap") {
                frameCount = 13;
                frameRate = 0.08f;
            }
            if (map == "ParadiseMap") {
                frameCount = 8;
                frameRate = 0.1f;
            }
        } else {
            enemyScale = 1.8f;
            if (map == "FireMap") {
                frameCount = 4;
                frameRate = 0.2f;
            }
            if (map == "HellMap") {
                frameCount = 6;
                frameRate = 0.1f;
            }
            if (map == "IceMap") {
                frameCount = 4;
                frameRate = 0.2f;
            }
            if (map == "ParadiseMap") {
                frameCount = 2;
                frameRate = 0.6f;
            }
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
