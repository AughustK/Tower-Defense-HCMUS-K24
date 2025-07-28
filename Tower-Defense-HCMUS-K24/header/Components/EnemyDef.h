#pragma once

#include "EnemyComponent.h"

/// Data-driven definition for each enemy type
struct EnemyDef
{
    EnemyComponent::EnemyType  type;
    float                      health;    
    float                      speed;     
    float                      damage;    
    float                      scale;
    int                        prize;
};

