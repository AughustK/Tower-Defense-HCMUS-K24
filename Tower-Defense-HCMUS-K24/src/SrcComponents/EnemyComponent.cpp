#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/EnemyDef.h"

// define the table exactly once
static const EnemyDef ENEMY_DEFS[] = {
    { EnemyComponent::EnemyType::Normal, 20.f, 80.f,  5.f },
    { EnemyComponent::EnemyType::Boss,   200.f,40.f, 20.f }
};

const EnemyDef& EnemyComponent::getEnemyDef(EnemyType t) {
    for (auto& def : ENEMY_DEFS)
        if (def.type == t) return def;
    return ENEMY_DEFS[0];
}

std::string EnemyComponent::getAnimationPath(EnemyType t, const string& map)
{
    std::string prefix;
    switch (t) {
    case EnemyType::Normal:
        prefix = "Normal";
        break;
    case EnemyType::Boss:
        prefix = "Boss";
        break;
    default:
        return "";
    }

    return "assets/" + map + "/" + prefix + "Spritesheet/" + "run.png";
}

void EnemyComponent::loadStats() {
    const EnemyDef& d = getEnemyDef(type);
    health = d.health;
    speed = d.speed;
    damage = d.damage;
}


