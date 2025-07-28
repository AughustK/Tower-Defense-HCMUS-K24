#include "../../header/Systems/TowerSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/TowerDef.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/CircleComponent.h"
#include "../../header/Systems/InitializeProjectile.h"

void TowerSystem::update(float deltaTime, World& world)
{
    auto& towerArray = world.getComponentArray<TowerComponent>();
    auto& circleArray = world.getComponentArray<CircleComponent>();
    auto& projectileArray = world.getComponentArray<ProjectileComponent>();
    auto& velocityArray = world.getComponentArray<VelocityComponent>();
    auto& positionArray = world.getComponentArray<PositionComponent>();

    for (auto const& [towerEnt, _] : towerArray.getEntityToIndexMap())
    {
        auto& T = towerArray.getData(towerEnt);
        auto& def = TOWER_DEFS[static_cast<int>(T.type)];

        // Use data-driven fire rate per level
        float fireRate = def.fireRate[T.level];
        if (fireRate <= 0.0f) continue;

        T.lastShotTimer += deltaTime;
        float interval = 1.0f / fireRate;
        if (T.lastShotTimer < interval) continue;

        // Find closest enemy within data-driven range
        EntityID target = 0;
        float range = def.range[T.level];
        float bestDist2 = range * range;
        for (auto const& [e, _2] : circleArray.getEntityToIndexMap())
        {
            auto& C = circleArray.getData(e);
            if (C.tag != CircleComponent::CollisionType::Enemy) continue;
            float dx = C.x - T.x;
            /*std::cout << "[DEBUG] Checking enemy at " << dx << std::endl;*/
            float dy = C.y - T.y;
            /*std::cout << "[DEBUG] Checking enemy at " << dy << std::endl;*/
            float dist2 = dx * dx + dy * dy;
            if (dist2 <= bestDist2)
            {
                bestDist2 = dist2;
                target = e;
            }
        }
        if (!target) continue;

        /// Compute velocity towards target
        auto& enemyC = circleArray.getData(target);
        float dx = enemyC.x - T.x;
        float dy = enemyC.y - T.y;
        float dist2 = dx * dx + dy * dy;
        float invLen = (dist2 > 0.0f) ? 1.0f / std::sqrt(dist2) : 0.0f;
        float dirX = dx * invLen;
        float dirY = dy * invLen;
        sf::Vector2f velocity = {
            dirX * def.projectileSpeed[T.level],
            dirY * def.projectileSpeed[T.level]
        };

        auto poolSystem = world.getSystem<ProjectilePoolSystem>();
        poolSystem->spawn(
            world,
            T.x,
            T.y,
            velocity,
            static_cast<ProjectileComponent::ProjectileType>(T.type),
            T.level,
            towerEnt
        );

        T.lastShotTimer = 0.0f;
    }
}

void TowerSystem::update(float dt)
{
}