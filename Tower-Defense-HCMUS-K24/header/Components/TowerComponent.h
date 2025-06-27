//#pragma once
//
//#include <cstdint>
//#include <limits>
//#include <string>
//
//struct TowerDef;
//
//using EntityID = uint32_t;
//constexpr EntityID INVALID_ENTITY = std::numeric_limits<EntityID>::max();
//
//struct TowerComponent 
//{
//    enum class TowerType : uint8_t { Archer = 0, Mage, Cannon };
//
//    // Upgrade level: 0 = base, 1 = upgraded
//    uint8_t     level = 0;
//    TowerType   type = TowerType::Archer;
//
//    // World position
//    float       x = 0.0f;
//    float       y = 0.0f;
//
//    // Runtime stats
//    float       fireRate = 0.0f;
//    float       projectileSpeed = 0.0f;
//    float       damage = 0.0f;
//    float       range = 0.0f;
//    float       projectileRadius = 0.0f;
//    float       projectileLength = 0.0f;
//    float       lastShotTimer = 0.0f;
//
//    // Current target (entity ID)
//    EntityID    target = INVALID_ENTITY;
//
//    TowerComponent(float tX = 0.0f, float tY = 0.0f, TowerType t = TowerType::Archer, uint8_t lvl = 0) : level(lvl), type(t), x(tX), y(tY) 
//    {
//        loadStats();
//    }
//
//    // Upgrade to next level (if available) and reload stats
//    void upgrade() {
//        if (level < 1) {
//            ++level;
//            loadStats();
//        }
//    }
//
//    // Load stats from data-driven TowerDef
//    void loadStats() {
//        const TowerDef& def = getTowerDef(type);
//        fireRate = def.fireRate[level];
//        projectileSpeed = def.projectileSpeed[level];
//        damage = def.damage[level];
//        range = def.range[level];
//        projectileRadius = def.projectileRadius;
//        projectileLength = def.projectileLength;
//        lastShotTimer = 0.0f;
//    }
//
//    // Fetch TowerDef for a given type
//    static const TowerDef& getTowerDef(TowerType t) {
//        return TOWER_DEFS[static_cast<size_t>(t)];
//    }
//
//    // Path-based sprite lookup: use with UISpriteComponent
//    static std::string getSpritePath(TowerType t, uint8_t lvl) {
//        switch (t) {
//        case TowerType::Archer:
//            return (lvl == 0)
//                ? "assets/towers/archer_base.png"
//                : "assets/towers/archer_up.png";
//        case TowerType::Mage:
//            return (lvl == 0)
//                ? "assets/towers/mage_base.png"
//                : "assets/towers/mage_up.png";
//        case TowerType::Cannon:
//            return (lvl == 0)
//                ? "assets/towers/cannon_base.png"
//                : "assets/towers/cannon_up.png";
//        }
//        return {};
//    }
//};

