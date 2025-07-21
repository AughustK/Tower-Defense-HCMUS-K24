#pragma once

#include "System.h"
#include "../Managers/World.h"
#include "../Managers/ComponentArray.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"


class PhysicSystem : public System
{
public:
    PhysicSystem() = default;
    virtual ~PhysicSystem() = default;

    void update(float deltaTime) override {}
    void update(float deltaTime, World& world);
    void update(float deltaTime, ComponentArray<PositionComponent>& positionArray, ComponentArray<VelocityComponent>& velocityArray);
};