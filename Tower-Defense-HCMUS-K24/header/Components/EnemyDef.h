#pragma once

#include "EnemyComponent.h"

/// Data-driven definition for each enemy type
struct EnemyDef
{
    EnemyComponent::EnemyType  type;
    float                      health;    // hit points
    float                      speed;     // units per second
    float                      damage;    // damage dealt on reach
};

