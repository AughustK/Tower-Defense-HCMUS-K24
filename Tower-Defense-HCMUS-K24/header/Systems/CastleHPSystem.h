#pragma once
#include <iostream>
#include "System.h"
#include "../Managers/World.h"

class CastleHPSystem : public System
{
public:
    CastleHPSystem() = default;

    void update(float deltaTime) override;
    void render(World& world);
    void handleEvent(World& world);
    void update(World& world, int dmg);
};
