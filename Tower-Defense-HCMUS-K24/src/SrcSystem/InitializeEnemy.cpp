#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"

void EnemySpawnSystem::update(float deltaTime){}

void EnemySpawnSystem::spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();

        PositionComponent posComp{};
        posComp.x = path.front().x;
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

        // 4) Health or Enemy tag
        HealthComponent hp{ /* starting HP */ };
        world.addComponent(e, hp);

        // (Optionally) Add a sprite so you can see them
        SpriteComponent sprite{ /* texture key, layer, etc. */ };
        world.addComponent(e, sprite);

        createdEnemies.push_back(e);
    }
}
