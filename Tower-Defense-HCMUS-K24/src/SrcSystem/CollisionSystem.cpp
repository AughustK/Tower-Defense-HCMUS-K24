#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Utils/Math.h"
#include "../../header/Components/TowerDef.h"
#include "../../header/Components/Buffs.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Systems/InitializeProjectile.h"
#include <cmath>
#include <iostream> // Added for debug output

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

    // Allocate a transient buffer for projectiles to destroy
    /*EntityID* toDestroy = static_cast<EntityID*>(
        World::transientArena.alloc(sizeof(EntityID) * MAX_PROJECTILES)
        );
    int destroyCount = 0;*/

    vector<EntityID> toDestroyEnemies;
    vector<EntityID> toHideProj;

    // Iterate all circles, filter projectiles
    for (auto& kv : circles)
    {
        EntityID projectile = kv.first;
        auto& cc = circleArray.getData(projectile);
        if (cc.tag != CircleComponent::CollisionType::Projectile) continue;
        if (!projectileArray.containData(projectile))      continue;


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

        auto applyBuff = [&](EntityID enemy, BuffType bType)
        {
            if (!buffArray.containData(enemy))
            {
                return;
            }

            auto& bc = buffArray.getData(enemy);
            const BuffDef& bDef = BUFF_DEFS[static_cast<size_t>(bType)];
            bc.buffs.push_back({ bType, bDef.duration, bDef.magnitude });
        };

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
                    if (Math::rectangleCircleSAT(cc.x, cc.y, halfLen, halfTh, angle, ec.x, ec.y, ec.radius))
                    {
                        /*std::cout << "[DEBUG] Arrow collision: Projectile " << projectile << " hit Enemy " << en << std::endl;*/
                        toHideProj.push_back(projectile);

                        if (healthArray.containData(en))
                        {
                            std::cout << "[CollisionSystem] Damaged taken\n";
                            auto& health = healthArray.getData(en);
                            health.takeDamage(damage);
                            if (health.isDead()){
                                std::cout << "[CollisionSystem] Entity " << en << " is pushed to destroy vector\n";
                                toDestroyEnemies.push_back(en);
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
                    float dx = cc.x - ec.x;
                    float dy = cc.y - ec.y;
                    if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
                    {
                        std::cout << "[DEBUG] MagicCircle collision: Projectile " << projectile << " hit Enemy " << en << std::endl;
                        toHideProj.push_back(projectile);
                        if (healthArray.containData(en))
                        {
                            auto& health = healthArray.getData(en);
                            health.takeDamage(damage);
                            if (health.isDead())
                                toDestroyEnemies.push_back(en);
                        }
                        // Apply stun buff
                        applyBuff(en, BuffType::Stun);
						std::cout << "[DEBUG] Applied Stun buff to Enemy " << en << std::endl;
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
                    float dx = cc.x - ec.x;
                    float dy = cc.y - ec.y;
                    if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
                    {
                        std::cout << "[DEBUG] CannonBall collision: Projectile " << projectile << " hit Enemy " << en << std::endl;
                        toHideProj.push_back(projectile);
                        if (healthArray.containData(en))
                        {
                            auto& health = healthArray.getData(en);
                            health.takeDamage(damage);
                            if (health.isDead())
                                toDestroyEnemies.push_back(en);
                        }
                        // Apply slow buff
                        applyBuff(en, BuffType::Slow);
                        break;
                    }
                }
            } break;
        }
    }
    for (auto& e : toHideProj) {
        world.getSystem<ProjectilePoolSystem>()->hideUsedProj(world, e);
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
