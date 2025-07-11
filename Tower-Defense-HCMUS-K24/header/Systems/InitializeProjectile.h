#pragma once

#pragma once
#include "System.h"
#include "../Managers/World.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/PositionComponent.h"

class ProjectilePoolSystem : public System 
{
private:
    std::vector<EntityID> createdProjectile;

public:

    void update(float deltaTime) {}

    void initPool(World& world, std::size_t count) 
    {
        for (std::size_t i = 0; i < count; ++i) 
        {
            auto e = world.createEntity();
            ProjectileComponent proj{};
            proj.x = proj.y = -100.f;                // off-screen
            proj.tag = ProjectileComponent::ProjectileType::Arrow;   // default type
            world.addComponent(e, proj);

            PositionComponent pos{ proj.x, proj.y, PositionComponent::Type::Projectile};
            world.addComponent(e, pos);
            createdProjectile.push_back(e);
        }
    }
};