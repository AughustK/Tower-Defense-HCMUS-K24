#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Utils/Math.h"
#include "../../header/Components/TowerDef.h"
#include "../../header/Components/Buffs.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/EnemyComponent.h"
#include "../../header/GameStates/GamePlay.h"
#include "../../header/Systems/InitializeProjectile.h"
#include "../../header/Components/EnemyDef.h"
#include "../../header/Systems/CastleHPSystem.h"
#include <cmath>
#include <iostream> // Added for debug output
#include <unordered_set> // Added for tracking enemies to destroy

float CollisionSystem::computeDistanceOfTwoPoint(float dX, float dY) const
{
    return std::sqrt(dX * dX + dY * dY);
}

void CollisionSystem::updateCheck(
    ComponentArray<VelocityComponent>& velocityArray,
    ComponentArray<CircleComponent>& circleArray,
    ComponentArray<ProjectileComponent>& projectileArray,
    ComponentArray<TowerComponent>& towerArray,
    ComponentArray<HealthComponent>& healthArray,
    ComponentArray<BuffComponent>& buffArray,
    World& world)
{
    const auto& circles = circleArray.getEntityToIndexMap();

    vector<EntityID> toDestroyEnemies;
    vector<EntityID> toHideProj;
    unordered_set<EntityID> enemiesToDestroy; // Track enemies already marked for destruction

    // Iterate all circles, filter projectiles
    for (auto& kv : circles)
    {
        EntityID projectile = kv.first;
        auto& cc = circleArray.getData(projectile);
        if (cc.tag != CircleComponent::CollisionType::Projectile) continue;
        if (!projectileArray.containData(projectile))      continue;

        //hide proj when passing through a killed enemy
        const float screenLeft = 0.f;
        const float screenRight = 1613.5f;
        const float screenTop = 0.f;
        const float screenBottom = 1080.f;

        if (cc.x < screenLeft || cc.x > screenRight || cc.y < screenTop || cc.y > screenBottom) {
            toHideProj.push_back(projectile);
            continue;
        }

        // Damage from owning tower
        int damage = 0;
        auto& pc = projectileArray.getData(projectile);
        if (towerArray.containData(pc.owner))
        {
            auto& towerComp = towerArray.getData(pc.owner);
            auto& def = TOWER_DEFS[static_cast<int>(towerComp.type)];
            damage = static_cast<int>(def.damage[towerComp.level]);
        }

        // Common collision parameters
        float pr = cc.radius;
        float angle = std::atan2(
            velocityArray.getData(projectile).y,
            velocityArray.getData(projectile).x
        );

        switch (pc.tag)
        {
        case ProjectileComponent::ProjectileType::Arrow:
        {
            float halfLen = pc.arrowLength * 0.5f;
            float halfTh = pr;
            for (auto& kv2 : circles)
            {
                EntityID en = kv2.first;
                auto& ec = circleArray.getData(en);
                if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
                if (enemiesToDestroy.find(en) != enemiesToDestroy.end()) continue; // Skip if already marked for destruction
                if (Math::rectangleCircleSAT(cc.x, cc.y, halfLen, halfTh, angle, ec.x, ec.y, ec.radius))
                {
                    toHideProj.push_back(projectile);

                    if (healthArray.containData(en))
                    {
                        auto& health = healthArray.getData(en);
                        health.takeDamage(damage);
                        if (health.isDead()) {
                            std::cout << "[CollisionSystem] Entity " << en << " is pushed to destroy vector\n";
                            if (enemiesToDestroy.find(en) == enemiesToDestroy.end()) {
                                toDestroyEnemies.push_back(en);
                                enemiesToDestroy.insert(en);
                                if (world.hasComponent<EnemyComponent>(en)) {
                                    auto& enemyComp = world.getComponent<EnemyComponent>(en);
                                    auto* gameplay = dynamic_cast<GamePlay*>(world.getCurrentState().get());
                                    int bonus = EnemyComponent::getEnemyDef(enemyComp.type).prize; 
                                    gameplay->updateMoney(bonus);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        } break;

        case ProjectileComponent::ProjectileType::MagicCircle:
        {
            for (auto& kv2 : circles)
            {
                EntityID en = kv2.first;
                auto& ec = circleArray.getData(en);
                if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
                if (enemiesToDestroy.find(en) != enemiesToDestroy.end()) continue; // Skip if already marked for destruction
                float dx = cc.x - ec.x;
                float dy = cc.y - ec.y;
                if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
                {
                    toHideProj.push_back(projectile);
                    if (healthArray.containData(en))
                    {
                        auto& health = healthArray.getData(en);
                        health.takeDamage(damage);
                        if (health.isDead()) {
                            std::cout << "[CollisionSystem] Entity " << en << " is pushed to destroy vector\n";
                            if (enemiesToDestroy.find(en) == enemiesToDestroy.end()) {
                                toDestroyEnemies.push_back(en);
                                enemiesToDestroy.insert(en);
                                if (world.hasComponent<EnemyComponent>(en)) {
                                    auto& enemyComp = world.getComponent<EnemyComponent>(en);
                                    auto* gameplay = dynamic_cast<GamePlay*>(world.getCurrentState().get());
                                    int bonus = EnemyComponent::getEnemyDef(enemyComp.type).prize; 
                                    gameplay->updateMoney(bonus);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        } break;

        case ProjectileComponent::ProjectileType::CannonBall:
        {
            for (auto& kv2 : circles)
            {
                EntityID en = kv2.first;
                auto& ec = circleArray.getData(en);
                if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
                if (enemiesToDestroy.find(en) != enemiesToDestroy.end()) continue; // Skip if already marked for destruction
                float dx = cc.x - ec.x;
                float dy = cc.y - ec.y;
                if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
                {
                    toHideProj.push_back(projectile);
                    if (healthArray.containData(en))
                    {
                        auto& health = healthArray.getData(en);
                        health.takeDamage(damage);
                        if (health.isDead()) {
                            std::cout << "[CollisionSystem] Entity " << en << " is pushed to destroy vector\n";
                            if (enemiesToDestroy.find(en) == enemiesToDestroy.end()) {
                                toDestroyEnemies.push_back(en);
                                enemiesToDestroy.insert(en);
                                if (world.hasComponent<EnemyComponent>(en)) {
                                    auto& enemyComp = world.getComponent<EnemyComponent>(en);
                                    auto* gameplay = dynamic_cast<GamePlay*>(world.getCurrentState().get());
                                    int bonus = EnemyComponent::getEnemyDef(enemyComp.type).prize; // Add a 'prize' field to EnemyDef if needed
                                    gameplay->updateMoney(bonus);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        } break;
        }
    }
    for (auto& e : toHideProj) {
        world.getSystem<ProjectilePoolSystem>()->hideUsedProj(world, e);
    }

    float castleRadius = 20.f;  

    for (auto& kv : circleArray.getEntityToIndexMap())
    {
        EntityID enemy = kv.first;
        auto& cc = circleArray.getData(enemy);
        if (cc.tag != CircleComponent::CollisionType::Enemy) continue;

        float dx = cc.x - currCastlePos.x;
        float dy = cc.y - currCastlePos.y;
        float distanceSq = dx * dx + dy * dy;

        float combinedRadius = cc.radius + castleRadius;

        if (distanceSq <= combinedRadius * combinedRadius)
        {
            std::cout << "[CollisionSystem] Enemy " << enemy << " reached the castle (within radius), destroying.\n";
            auto& enemyComp = world.getComponent<EnemyComponent>(enemy);
            int damage = static_cast<int>(EnemyComponent::getEnemyDef(enemyComp.type).damage);

            world.getSystem<CastleHPSystem>()->update(world, damage);
            toDestroyEnemies.push_back(enemy);
        }
    }


    for (auto& e : toDestroyEnemies) {
        world.destroyEntity(e);
    }
    toDestroyEnemies.clear();
}

void CollisionSystem::update(float deltaTime, World& world)
{
    auto& velocityArray = world.getComponentArray<VelocityComponent>();
    auto& circleArray = world.getComponentArray<CircleComponent>();
    auto& projectileArray = world.getComponentArray<ProjectileComponent>();
    auto& towerArray = world.getComponentArray<TowerComponent>();
    auto& healthArray = world.getComponentArray<HealthComponent>();
    auto& buffArray = world.getComponentArray<BuffComponent>();

    updateCheck(velocityArray,
        circleArray,
        projectileArray,
        towerArray,
        healthArray,
        buffArray,
        world);
}

void CollisionSystem::init(string name) {
    castlePos["FireMap"] = { 1320,825 };
    castlePos["HellMap"] = {1210, 380};
    castlePos["IceMap"] = { 1290, 785 };
    castlePos["ParadiseMap"] = { 1160, 585 };
    currCastlePos= castlePos[name];
}