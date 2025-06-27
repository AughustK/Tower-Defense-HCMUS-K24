//#include "../../header/Systems/CollisionSystem.h"
//#include "../../header/Managers/World.h"
//#include "../../header/Utils/Math.h"
//#include "../../header/Components/TowerDef.h"
//#include <cmath>
//
//float CollisionSystem::computeDistanceOfTwoPoint(float dX, float dY) const
//{
//    return std::sqrt(dX * dX + dY * dY);
//}
//
//void CollisionSystem::updateCheck(
//    ComponentArray<VelocityComponent>& velocityArray,
//    ComponentArray<CircleComponent>& circleArray,
//    ComponentArray<ProjectileComponent>& projectileArray,
//    ComponentArray<TowerComponent>& towerArray,
//    ComponentArray<HealthComponent>& healthArray,
//    ComponentArray<BuffComponent>& buffArray,
//    EntityManager& entityManager)
//{
//    const auto& circles = circleArray.getEntityToIndexMap();
//
//    // Allocate a transient buffer for projectiles to destroy
//    EntityID* toDestroy = static_cast<EntityID*>(
//        World::transientArena.alloc(sizeof(EntityID) * MAX_PROJECTILES)
//        );
//    int destroyCount = 0;
//
//    // Iterate all circles, filter projectiles
//    for (auto& kv : circles)
//    {
//        EntityID projectile = kv.first;
//        auto& cc = circleArray.getData(projectile);
//        if (cc.tag != CircleComponent::CollisionType::Projectile) continue;
//        if (!projectileArray.containData(projectile))      continue;
//
//
//        // Damage from owning tower
//        int damage = 0;
//        auto& pc = projectileArray.getData(projectile);
//        if (towerArray.containData(pc.owner))
//        {
//            auto& towerComp = towerArray.getData(pc.owner);
//            auto& def = TOWER_DEFS[static_cast<int>(towerComp.type)];
//            damage = static_cast<int>(def.damage[towerComp.level]);
//        }
//
//        // Common collision parameters
//        float pr = cc.radius;
//        float angle = std::atan2(
//            velocityArray.getData(projectile).y,
//            velocityArray.getData(projectile).x
//        );
//
//        switch (pc.tag)
//        {
//            case ProjectileComponent::ProjectileType::Arrow:
//            {
//                float halfLen = pc.arrowLength * 0.5f;
//                float halfTh = pr;
//                for (auto& kv2 : circles)
//                {
//                    EntityID en = kv2.first;
//                    auto& ec = circleArray.getData(en);
//                    if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
//                    if (Math::rectangleCircleSAT(cc.x, cc.y, halfLen, halfTh, angle, ec.x, ec.y, ec.radius))
//                    {
//                        if (healthArray.containData(en))
//                        {
//                            auto& health = healthArray.getData(en);
//                            health.takeDamage(damage);
//                            if (health.isDead())
//                                entityManager.destroyEntity(en);
//                        }
//                        toDestroy[destroyCount++] = projectile;
//                        break;
//                    }
//                }
//            } break;
//
//            case ProjectileComponent::ProjectileType::MagicCircle:
//            {
//                for (auto& kv2 : circles)
//                {
//                    EntityID en = kv2.first;
//                    auto& ec = circleArray.getData(en);
//                    if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
//                    float dx = cc.x - ec.x;
//                    float dy = cc.y - ec.y;
//                    if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
//                    {
//                        if (healthArray.containData(en))
//                        {
//                            auto& health = healthArray.getData(en);
//                            health.takeDamage(damage);
//                            if (health.isDead())
//                                entityManager.destroyEntity(en);
//                        }
//                        // Apply stun buff
//                        if (buffArray.containData(en))
//                        {
//                            auto& bc = buffArray.getData(en);
//                            bc.buffs.push_back({ BuffType::Stun, 0.0f, 0.0f });
//                        }
//                        toDestroy[destroyCount++] = projectile;
//                        break;
//                    }
//                }
//            } break;
//
//            case ProjectileComponent::ProjectileType::CannonBall:
//            {
//                for (auto& kv2 : circles)
//                {
//                    EntityID en = kv2.first;
//                    auto& ec = circleArray.getData(en);
//                    if (ec.tag != CircleComponent::CollisionType::Enemy) continue;
//                    float dx = cc.x - ec.x;
//                    float dy = cc.y - ec.y;
//                    if (dx * dx + dy * dy <= (pr + ec.radius) * (pr + ec.radius))
//                    {
//                        if (healthArray.containData(en))
//                        {
//                            auto& health = healthArray.getData(en);
//                            health.takeDamage(damage);
//                            if (health.isDead())
//                                entityManager.destroyEntity(en);
//                        }
//                        // Apply slow buff
//                        if (buffArray.containData(en))
//                        {
//                            auto& bc = buffArray.getData(en);
//                            bc.buffs.push_back({ BuffType::Slow, 0.0f, 0.0f });
//                        }
//                        toDestroy[destroyCount++] = projectile;
//                        break;
//                    }
//                }
//            } break;
//        }
//
//        // Destroy all projectiles that hit this cycle
//        for (int i = 0; i < destroyCount; ++i)
//        {
//            entityManager.destroyEntity(toDestroy[i]);
//        }
//        destroyCount = 0;
//    }
//}
