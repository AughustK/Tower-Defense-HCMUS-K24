#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();

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

        HealthComponent hp{ static_cast<int>(enemy.health) };
        world.addComponent(e, hp);

        
		std::string spritePath = EnemyComponent::getAnimationPath(enemy.type, map);
		
        float enemyScale = 0.3f;
        if(enemy.type == EnemyComponent::EnemyType::Boss)
        {
            enemyScale = 1.8f; 
		}

        SpriteComponent spriteE(spritePath, { posComp.x, posComp.y }, { enemyScale, enemyScale });
        
        if (spriteE.texture && spriteE.texture->getSize().x > 0)
        {
            const int FRAME_COUNT = 4;
            int frameWidth = spriteE.texture->getSize().x / FRAME_COUNT;
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
		cout << "Destroying enemy entity: " << enemy << "\n";
        world.destroyEntity(enemy);
    }
	createdEnemies.clear();
}
