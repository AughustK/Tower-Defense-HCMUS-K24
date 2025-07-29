#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/EnemyDef.h"

// define the table exactly once
static const EnemyDef ENEMY_DEFS[] = {
    { EnemyComponent::EnemyType::FireNormal1, 800.f, 80.f,  100.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::FireNormal2, 800.f, 80.f,  100.f, 0.5f, 10 },
    { EnemyComponent::EnemyType::FireBoss,   1800.f,40.f, 400.f, 1.2f, 100 },
    { EnemyComponent::EnemyType::IceNormal1, 800.f, 80.f,  100.f, 1.3f, 10 },
    { EnemyComponent::EnemyType::IceNormal2, 800.f, 80.f,  100.f, 1.0f, 10 },
    { EnemyComponent::EnemyType::IceBoss,   1800.f,40.f, 400.f, 3.0f, 100 },
    { EnemyComponent::EnemyType::ParadiseNormal1, 800.f, 80.f,  100.f, 1.0f, 10 },
    { EnemyComponent::EnemyType::ParadiseNormal2, 800.f, 80.f,  100.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::ParadiseBoss,   1800.f,40.f, 400.f, 1.4f, 100 },
    { EnemyComponent::EnemyType::HellNormal1, 800.f, 80.f,  100.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::HellNormal2, 800.f, 80.f,  100.f, 0.6f, 10 },
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
    case EnemyType::FireNormal1:
        prefix = "FireNormal1";
        break;
    case EnemyType::FireNormal2:
        prefix = "FireNormal2";
        break;
    case EnemyType::FireBoss:
        prefix = "FireBoss";
        break;
    case EnemyType::IceNormal1:
        prefix = "IceNormal1";
        break;
    case EnemyType::IceNormal2:
        prefix = "IceNormal2";
        break;
    case EnemyType::IceBoss:
        prefix = "IceBoss";
        break;
    case EnemyType::ParadiseNormal1:
        prefix = "ParadiseNormal1";
        break;
    case EnemyType::ParadiseNormal2:
        prefix = "ParadiseNormal2";
        break;
    case EnemyType::ParadiseBoss:
        prefix = "ParadiseBoss";
        break;
    case EnemyType::HellNormal1:
        prefix = "HellNormal1";
        break;
    case EnemyType::HellNormal2:
        prefix = "HellNormal2";
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


