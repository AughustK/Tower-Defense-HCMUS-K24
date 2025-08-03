#include "../../header/Systems/TowerSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/TowerDef.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/CircleComponent.h"
#include "../../header/Systems/InitializeProjectile.h"

static float solveInterceptTime(
    float rdx, float rdy,     // vector from tower to enemy: Epos - Tpos
    float evx, float evy,     // enemy velocity
    float projSpeed)          // projectile speed
{
    float a = evx * evx + evy * evy - projSpeed * projSpeed;
    float b = 2 * (rdx * evx + rdy * evy);
    float c = rdx * rdx + rdy * rdy;

    float disc = b * b - 4 * a * c;
    if (disc < 0 || fabs(a) < 1e-6f) {
        // no real solution or degenerate fall back to direct aim
        return (c > 0 && b < 0) ? -c / b : -1.f;
    }
    float sq = std::sqrt(disc);
    float t0 = (-b + sq) / (2 * a);
    float t1 = (-b - sq) / (2 * a);

    // choose smallest positive
    float t = INFINITY;
    if (t0 > 0) t = t0;
    if (t1 > 0 && t1 < t) t = t1;
    return (t < INFINITY ? t : -1.f);
}

void TowerSystem::update(float deltaTime, World& world)
{
    auto& towers = world.getComponentArray<TowerComponent>();
    auto& circles = world.getComponentArray<CircleComponent>();
    auto& velocities = world.getComponentArray<VelocityComponent>();

    for (auto const& [towerEnt, _] : towers.getEntityToIndexMap())
    {
        auto& T = towers.getData(towerEnt);
        const auto& def = TOWER_DEFS[static_cast<int>(T.type)];

        // Fire rate gating
        float rate = def.fireRate[T.level];
        if (rate <= 0) continue;
        T.lastShotTimer += deltaTime;
        if (T.lastShotTimer < 1.0f / rate) continue;

        // Find closest enemy in range
        EntityID target = INVALID_ENTITY;
        float bestDist2 = def.range[T.level] * def.range[T.level];
        for (auto const& [e, _2] : circles.getEntityToIndexMap())
        {
            auto& C = circles.getData(e);
            if (C.tag != CircleComponent::CollisionType::Enemy) continue;

            float dx = C.x - T.x;
            float dy = C.y - T.y;
            float d2 = dx * dx + dy * dy;
            if (d2 <= bestDist2) {
                bestDist2 = d2;
                target = e;
            }
        }
        if (target == INVALID_ENTITY) continue;

        // Gather enemy data
        auto& C = circles.getData(target);
        auto& Vc = velocities.getData(target);
        sf::Vector2f Ev{ Vc.x, Vc.y };
        sf::Vector2f r{ C.x - T.x, C.y - T.y };

        // Solve for intercept time
        float vp = def.projectileSpeed[T.level];
        float t = solveInterceptTime(r.x, r.y, Ev.x, Ev.y, vp);
        if (t <= 0) {
            // fallback: aim directly at current pos
            t = std::sqrt(bestDist2) / vp;
        }

        // Compute aim point and shot velocity
        sf::Vector2f aim{
            C.x + Ev.x * t,
            C.y + Ev.y * t
        };
        sf::Vector2f shotVel = (aim - sf::Vector2f{ T.x, T.y }) * (1.0f / t);

        // Spawn the projectile
        auto pool = world.getSystem<ProjectilePoolSystem>();
        pool->spawn(
            world,
            T.x, T.y,
            shotVel,
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
