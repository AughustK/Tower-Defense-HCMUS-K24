#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include "../Managers/EntityManager.h"

struct TowerDef;

struct TowerComponent 
{
    enum class TowerType : uint8_t { Archer = 0, Mage, Cannon };

    // Upgrade level: 0 = base, 1 = upgraded
    uint8_t     level = 0;
    TowerType   type = TowerType::Archer;

    // World position
    float       x = 0.0f;
    float       y = 0.0f;

    // Runtime stats
    float       fireRate = 0.0f;
    float       projectileSpeed = 0.0f;
    float       damage = 0.0f;
    float       range = 0.0f;
    float       projectileRadius = 0.0f;
    float       projectileLength = 0.0f;
    float       lastShotTimer = 0.0f;

	int cost = 0; 
    // Current target (entity ID)
    EntityID    target = INVALID_ENTITY;

    TowerComponent(float tX = 0.0f, float tY = 0.0f, TowerType t = TowerType::Archer, uint8_t lvl = 0) : level(lvl), type(t), x(tX), y(tY) 
    {
        loadStats();
    }

    // Upgrade to next level (if available) and reload stats
    void upgrade();

    // Load stats from data-driven TowerDef
    void loadStats();

    // Fetch TowerDef for a given type
    static const TowerDef& getTowerDef(TowerType t);

    // Path-based sprite lookup: use with UISpriteComponent
    static std::string getSpritePath(TowerType t, uint8_t lvl);
};

