#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count)
{
    const int TEST_MAP_ID = 1; // temp mapId for testing
    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();

        PositionComponent posComp{};
        posComp.x = path.front().x; // Spawn point
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
        enemy.type = (i % 5 == 0) ? EnemyComponent::EnemyType::Boss : EnemyComponent::EnemyType::Normal;
        enemy.speed = 10.0;
        enemy.loadStats(); // Set health, speed, etc...
        pathComp.speed = enemy.speed;
        world.addComponent(e, enemy);

        HealthComponent hp{static_cast<int>(enemy.health)};
        world.addComponent(e, hp);

        // std::string spritePath = enemy.getSpritePath(enemy.type, TEST_MAP_ID);
        std::string spritePath = "assets/FireMap/Bot1/1_enemies_1_jump_spritesheet.png";
        
        SpriteComponent sprite(spritePath, {posComp.x, posComp.y}, {0.3f, 0.3f});
        world.addComponent(e, sprite);

        createdEnemies.push_back(e);
    }
}
