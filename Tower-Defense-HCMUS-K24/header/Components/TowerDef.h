#pragma once
#include "TowerComponent.h"



/// Data-driven definition for each tower, including base (level 0) and upgraded (level 1) stats
struct TowerDef 
{
    TowerComponent::TowerType type;

    // [0] = base, [1] = upgraded
    float fireRate[2];         // shots per second
    float projectileSpeed[2];  // units/sec
    float damage[2];           // hit points per shot
    float range[2];            // targeting radius

    // collision parameters (same for both levels)
    float projectileRadius;    // for circle collision
    float projectileLength;    // for OBB (arrows)
    float scale[2];
};

static constexpr TowerDef TOWER_DEFS[] = 
{
    //           type                 fireRate       projSpeed        damage         range       projRad  projLen
    { TowerComponent::TowerType::Archer, {1.0f, 1.2f},  {300.0f, 350.0f},  {10.0f, 15.0f}, {200.0f, 220.0f}, 5.0f,    14.0f, {3.5f, 2.5f} },
    { TowerComponent::TowerType::Mage,   {0.8f, 1.0f},  {250.0f, 300.0f},  {20.0f, 30.0f}, {180.0f, 200.0f}, 6.0f,     0.0f, {2.5f, 2.5f} },
    { TowerComponent::TowerType::Cannon, {0.5f, 0.7f},  {200.0f, 250.0f},  {50.0f, 75.0f}, {220.0f, 240.0f}, 8.0f,     0.0f, {1.0f, 1.0f} },
};