#pragma once

#include "EnemyComponent.h"
#include "Difficulty.h"


/// Data-driven definition for each enemy type
struct EnemyDef
{
    EnemyComponent::EnemyType  type;
    float                      baseHealth;
    float                      baseSpeed;
    float                      baseDamage;
    float                      scale;
    int                        basePrize;

    // Multipliers for each difficulty: [Easy, Normal, Hard]
    float healthMultiplier[3]   = {1.0f, 1.2f, 1.5f};
    float speedMultiplier[3]    = {1.0f, 1.1f, 1.1f};
    float damageMultiplier[3]   = {1.0f, 1.0f, 2.0f};
    float prizeMultiplier[3]    = {1.0f, 1.2f, 1.5f};

    float getHealth(DifficultyLevel diff) const {
        return baseHealth * healthMultiplier[static_cast<int>(diff)];
    }
    float getSpeed(DifficultyLevel diff) const {
        return baseSpeed * speedMultiplier[static_cast<int>(diff)];
    }
    float getDamage(DifficultyLevel diff) const {
        return baseDamage * damageMultiplier[static_cast<int>(diff)];
    }
    int getPrize(DifficultyLevel diff) const {
        return static_cast<int>(basePrize * prizeMultiplier[static_cast<int>(diff)]);
    }
};

