#include "../../header/Components/TowerComponent.h"
#include "../../header/Components/TowerDef.h"

void TowerComponent::upgrade()
{
    if (level < 1) 
    {
        ++level;
        loadStats();
    }
}

void TowerComponent::loadStats()
{
    const TowerDef& def = getTowerDef(type);
    fireRate = def.fireRate[level];
    projectileSpeed = def.projectileSpeed[level];
    damage = def.damage[level];
    range = def.range[level];
    projectileRadius = def.projectileRadius;
    projectileLength = def.projectileLength;
    lastShotTimer = 0.0f;
    cost = def.cost[level];
}

const TowerDef& TowerComponent::getTowerDef(TowerType t)
{
    return TOWER_DEFS[static_cast<size_t>(t)];
}

std::string TowerComponent::getSpritePath(TowerType t, uint8_t lvl)
{
    switch (t) 
    {
    case TowerType::Archer:
        return (lvl == 0)
            ? "assets/heroes/archer_base.png"
            : "assets/heroes/archer_up.png";
    case TowerType::Mage:
        return (lvl == 0)
            ? "assets/heroes/mage_base.png"
            : "assets/heroes/mage_up.png";
    case TowerType::Cannon:
        return (lvl == 0)
            ? "assets/heroes/cannon_base.png"
            : "assets/heroes/cannon_up.png";
    }
    return {};
}





