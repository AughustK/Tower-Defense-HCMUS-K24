#pragma once

#include <array>
#include <unordered_map>

#include "../Utils/Math.h"
#include "../Managers/ComponentArray.h"
#include "../Components/CircleComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TowerComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Managers/EntityManager.h"
#include "System.h"
#include "../Managers/World.h"
using namespace std;

static constexpr size_t MAX_PROJECTILES = 1024;

class CollisionSystem : public System
{
private: 
    std::unordered_map<std::string, sf::Vector2f> castlePos;
    sf::Vector2f currCastlePos;
    vector<EntityID> soundEntities;

public:
    void init(string name);

    // Utility: straight?line distance
    float computeDistanceOfTwoPoint(float dX, float dY) const;
    
    // Play collision sound effect
    void playCollisionSound(World& world, ProjectileComponent::ProjectileType type);

    // Main collision check: now includes projectileArray
    void updateCheck(
        ComponentArray<VelocityComponent>& velocityArray,
        ComponentArray<CircleComponent>& circleArray,
        ComponentArray<ProjectileComponent>& projectileArray,
        ComponentArray<TowerComponent>& towerArray,
        ComponentArray<HealthComponent>& healthArray,
        World& world);

    void update(float deltaTime, World& world);
    void update(float dt) override;
};
