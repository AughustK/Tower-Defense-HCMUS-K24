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

    //cost
    int cost[2];
};

static constexpr TowerDef TOWER_DEFS[] =
{
    //           type                       fireRate       projSpeed           damage              range      projRad    projLen  scale             cost
    { TowerComponent::TowerType::Archer, {0.8f, 1.3f},  {200.0f, 350.0f},  {120.0f, 220.0f}, {140.0f, 240.0f}, 5.0f,    14.0f, {3.2f, 2.2f},      {20, 30} },
    { TowerComponent::TowerType::Mage,   {1.0f, 1.8f},  {250.0f, 300.0f},  {170.0f, 330.0f}, {160.0f, 220.0f}, 6.0f,     0.0f, {2.2f, 2.2f},      {30, 40} },
    { TowerComponent::TowerType::Cannon, {0.8f, 1.2f},  {200.0f, 250.0f},  {270.0f, 470.0f}, {200.0f, 260.0f}, 8.0f,     0.0f, {0.8f, 1.0f},      {40, 50} },
};