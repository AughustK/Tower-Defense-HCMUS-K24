#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <iostream>
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
    static std::string getAnimationPath(TowerType t, uint8_t lvl);
};

inline std::ostream& operator<<(std::ostream& os, const TowerComponent& t) {
    os << "TowerComponent\n";
    os << static_cast<int>(t.type) << '\n';
    os << static_cast<int>(t.level) << '\n';
    os << t.x << '\n';
    os << t.y << '\n';
    os << t.fireRate << '\n';
    os << t.projectileSpeed << '\n';
    os << t.damage << '\n';
    os << t.range << '\n';
    os << t.projectileRadius << '\n';
    os << t.projectileLength << '\n';
    os << t.lastShotTimer << '\n';
    os << t.cost << '\n';
    os << static_cast<int>(t.target) << '\n'; // EntityID -> int
    return os;
}

inline std::istream& operator>>(std::istream& is, TowerComponent& t) {
    int typeInt = 0;
    int levelInt = 0;
    int targetInt = 0;

    is >> typeInt;
    is >> levelInt;
    t.type = static_cast<TowerComponent::TowerType>(typeInt);
    t.level = static_cast<uint8_t>(levelInt);

    is >> t.x;
    is >> t.y;
    is >> t.fireRate;
    is >> t.projectileSpeed;
    is >> t.damage;
    is >> t.range;
    is >> t.projectileRadius;
    is >> t.projectileLength;
    is >> t.lastShotTimer;
    is >> t.cost;
    is >> targetInt;
    t.target = static_cast<EntityID>(targetInt);

    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return is;
}




