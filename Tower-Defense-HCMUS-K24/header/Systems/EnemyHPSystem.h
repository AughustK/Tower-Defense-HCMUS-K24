#pragma once
#include "System.h"
#include "../Managers/World.h"

class EnemyHPSystem : public System {
public:
    EnemyHPSystem() = default;

    void update(float deltaTime) override;
    void update(World& world, float dt);
    void render(World& world);
};
