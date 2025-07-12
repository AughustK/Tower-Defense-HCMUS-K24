#pragma once

#include <array>

#include "../Utils/Math.h"
#include "../Managers/ComponentArray.h"
#include "../Components/CircleComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TowerComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/Buffs.h"
#include "../Managers/EntityManager.h"
#include "System.h"
#include "../Managers/World.h"

static constexpr size_t MAX_PROJECTILES = 1024;

class CollisionSystem : public System
{
public:
    // Utility: straight?line distance
    float computeDistanceOfTwoPoint(float dX, float dY) const;

    // Main collision check: now includes projectileArray
    void updateCheck(
        ComponentArray<VelocityComponent>& velocityArray,
        ComponentArray<CircleComponent>& circleArray,
        ComponentArray<ProjectileComponent>& projectileArray,
        ComponentArray<TowerComponent>& towerArray,
        ComponentArray<HealthComponent>& healthArray,
        ComponentArray<BuffComponent>& buffArray,
        EntityManager& entityManager
    );  
    void update(float deltaTime, World& world);
    void update(float deltaTime) {};
};
