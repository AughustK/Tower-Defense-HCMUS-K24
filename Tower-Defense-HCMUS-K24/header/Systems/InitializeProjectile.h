#pragma once

#include "System.h"
#include "../Managers/World.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/UISpriteComponent.h"

class ProjectilePoolSystem : public System 
{
private:
    std::vector<EntityID> createdProjectile;
    std::size_t nextIndex = 0;

public:

    void update(float deltaTime) {}

    void initPool(World& world, std::size_t count);
    EntityID spawn(World& world, float startX, float startY, const sf::Vector2f& velocity, ProjectileComponent::ProjectileType type);
};