#include "../../header/Systems/TowerSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/TowerDef.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/CircleComponent.h"

void TowerSystem::initializePool(long long poolSize, World& world) 
{
    projectilePool.clear();
    projectilePool.reserve(poolSize);
    for (long long i = 0; i < poolSize; i++) {
        EntityID projectile = world.createEntity();
        world.addComponent(projectile, PositionComponent());
        world.addComponent(projectile, VelocityComponent());
        world.addComponent(projectile, ProjectileComponent());
        world.addComponent(projectile, CircleComponent());
        projectilePool.push_back(projectile);
    }
}

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
			std::cout << "[DEBUG] Checking enemy at " << dx << std::endl;
            float dy = C.y - T.y;
			std::cout << "[DEBUG] Checking enemy at " << dy << std::endl;
            float dist2 = dx * dx + dy * dy;
            if (dist2 <= bestDist2)
            {
                bestDist2 = dist2;
                target = e;
				std::cout << "[DEBUG] Found target: " << target << " at distance " << std::sqrt(dist2) << std::endl;
            }
        }
        if (!target) continue;

        // Debug: print when tower fires
        std::cout << "[DEBUG] Tower " << towerEnt << " (type=" << int(T.type) << ", level=" << T.level << ") fires at Enemy " << target << std::endl;

        // Recycle next projectile from the pool
        EntityID P = projectilePool[nextProjectile];
        nextProjectile = (nextProjectile + 1) % projectilePool.size();

        // Reset Position
        auto& pos = positionArray.getData(P);
        pos.x = T.x;
        pos.y = T.y;
        pos.type = PositionComponent::Type::Projectile;

        // Compute velocity towards target
        auto& vel = velocityArray.getData(P);
        auto& enemyC = circleArray.getData(target);
        float dx = enemyC.x - T.x;
        float dy = enemyC.y - T.y;
        float dist2 = dx * dx + dy * dy;
        float invLen;
        if (dist2 > 0.0f) 
        {
            invLen = 1.0f / std::sqrt(dist2);
        }
        else {
            invLen = 0.0f;
        }
        float dirX = dx * invLen;
        float dirY = dy * invLen;
        vel.x = dirX * def.projectileSpeed[T.level];
        vel.y = dirY * def.projectileSpeed[T.level];

        // Reset Projectile component
        auto& pc = projectileArray.getData(P);
        pc.tag = static_cast<ProjectileComponent::ProjectileType>(T.type);
        pc.owner = towerEnt;
        pc.x = T.x;
        pc.y = T.y;
        if (T.type == TowerComponent::TowerType::Archer) 
        {
            pc.arrowLength = def.projectileLength;
        }
        else {
            pc.arrowLength = 0.0f;
        }

        // Reset Collision circle
        auto& cc = circleArray.getData(P);
        cc.x = T.x;
        cc.y = T.y;
        cc.tag = CircleComponent::CollisionType::Projectile;
        cc.radius = def.projectileRadius;

        // Ready for next shot
        T.lastShotTimer = 0.0f;
    }
}

void TowerSystem::update(float dt)
{
}
