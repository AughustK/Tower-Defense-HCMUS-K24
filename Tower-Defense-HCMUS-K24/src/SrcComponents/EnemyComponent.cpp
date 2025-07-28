#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/EnemyDef.h"

// define the table exactly once
static const EnemyDef ENEMY_DEFS[] = {
    { EnemyComponent::EnemyType::FireNormal, 800.f, 80.f,  100.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::FireBoss,   1800.f,40.f, 400.f, 1.2f, 100 },
    { EnemyComponent::EnemyType::IceNormal, 800.f, 80.f,  100.f, 1.3f, 10 },
    { EnemyComponent::EnemyType::IceBoss,   1800.f,40.f, 400.f, 3.0f, 100 },
    { EnemyComponent::EnemyType::ParadiseNormal, 800.f, 80.f,  100.f, 1.0f, 10 },
    { EnemyComponent::EnemyType::ParadiseBoss,   1800.f,40.f, 400.f, 1.4f, 100 },
    { EnemyComponent::EnemyType::HellNormal, 800.f, 80.f,  100.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::HellBoss,   1800.f,40.f, 400.f, 1.2f, 100 }
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
    case EnemyType::FireNormal:
        prefix = "FireNormal";
        break;
    case EnemyType::FireBoss:
        prefix = "FireBoss";
        break;
    case EnemyType::IceNormal:
        prefix = "IceNormal";
        break;
    case EnemyType::IceBoss:
        prefix = "IceBoss";
        break;
    case EnemyType::ParadiseNormal:
        prefix = "ParadiseNormal";
        break;
    case EnemyType::ParadiseBoss:
        prefix = "ParadiseBoss";
        break;
    case EnemyType::HellNormal:
        prefix = "HellNormal";
        break;
    case EnemyType::HellBoss:
        prefix = "HellBoss";
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
    scale = d.scale;
}


