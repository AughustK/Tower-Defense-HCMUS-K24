//#include "../../header/Systems/TowerSystem.h"
//#include "../../header/Managers/World.h"
//#include "../../header/Components/TowerDef.h"
//#include "../../header/Components/PositionComponent.h"
//#include "../../header/Components/VelocityComponent.h"
//#include "../../header/Components/ProjectileComponent.h"
//#include "../../header/Components/CircleComponent.h"
//
//void TowerSystem::initializePool(long long poolSize)
//{
//    projectilePool.clear();
//    projectilePool.reserve(poolSize);
//
//    for (long long i = 0; i < poolSize; i++)
//    {
//        EntityID projectile = entityManager.createEntity();
//        componentManager.addComponent<PositionComponent>(projectile, PositionComponent());
//        componentManager.addComponent<VelocityComponent>(projectile, VelocityComponent());
//        componentManager.addComponent<ProjectileComponent>(projectile, ProjectileComponent());
//        componentManager.addComponent<CircleComponent>(projectile, CircleComponent());
//        projectilePool.push_back(projectile);
//    }
//}
//
//void TowerSystem::update(float deltaTime,
//    ComponentArray<TowerComponent>& towerArray,
//    ComponentArray<CircleComponent>& circleArray,
//    ComponentArray<ProjectileComponent>& projectileArray,
//    ComponentArray<VelocityComponent>& velocityArray,
//    ComponentArray<PositionComponent>& positionArray)
//{
//    for (auto const& [towerEnt, _] : towerArray.getEntityToIndexMap())
//    {
//        auto& T = towerArray.getData(towerEnt);
//        auto& def = TOWER_DEFS[static_cast<int>(T.type)];
//
//        // Use data-driven fire rate per level
//        float fireRate = def.fireRate[T.level];
//        if (fireRate <= 0.0f) continue;
//
//        T.lastShotTimer += deltaTime;
//        float interval = 1.0f / fireRate;
//        if (T.lastShotTimer < interval) continue;
//
//        // Find closest enemy within data-driven range
//        EntityID target = 0;
//        float range = def.range[T.level];
//        float bestDist2 = range * range;
//        for (auto const& [e, _2] : circleArray.getEntityToIndexMap())
//        {
//            auto& C = circleArray.getData(e);
//            if (C.tag != CircleComponent::CollisionType::Enemy) continue;
//            float dx = C.x - T.x;
//            float dy = C.y - T.y;
//            float dist2 = dx * dx + dy * dy;
//            if (dist2 <= bestDist2)
//            {
//                bestDist2 = dist2;
//                target = e;
//            }
//        }
//        if (!target) continue;
//
//        // Recycle next projectile from the pool
//        EntityID P = projectilePool[nextProjectile];
//        nextProjectile = (nextProjectile + 1) % projectilePool.size();
//
//        // Reset Position
//        auto& pos = positionArray.getData(P);
//        pos.x = T.x;
//        pos.y = T.y;
//        pos.type = PositionComponent::Type::Projectile;
//
//        // Compute velocity towards target
//        auto& vel = velocityArray.getData(P);
//        auto& enemyC = circleArray.getData(target);
//        float dx = enemyC.x - T.x;
//        float dy = enemyC.y - T.y;
//        float dist2 = dx * dx + dy * dy;
//        float invLen;
//        if (dist2 > 0.0f) 
//        {
//            invLen = 1.0f / std::sqrt(dist2);
//        }
//        else {
//            invLen = 0.0f;
//        }
//        float dirX = dx * invLen;
//        float dirY = dy * invLen;
//        vel.x = dirX * def.projectileSpeed[T.level];
//        vel.y = dirY * def.projectileSpeed[T.level];
//
//        // Reset Projectile component
//        auto& pc = projectileArray.getData(P);
//        pc.tag = static_cast<ProjectileComponent::ProjectileType>(T.type);
//        pc.owner = towerEnt;
//        pc.x = T.x;
//        pc.y = T.y;
//        if (T.type == TowerComponent::TowerType::Archer) 
//        {
//            pc.arrowLength = def.projectileLength;
//        }
//        else {
//            pc.arrowLength = 0.0f;
//        }
//
//        // Reset Collision circle
//        auto& cc = circleArray.getData(P);
//        cc.x = T.x;
//        cc.y = T.y;
//        cc.tag = CircleComponent::CollisionType::Projectile;
//        cc.radius = def.projectileRadius;
//
//        // Ready for next shot
//        T.lastShotTimer = 0.0f;
//    }
//}