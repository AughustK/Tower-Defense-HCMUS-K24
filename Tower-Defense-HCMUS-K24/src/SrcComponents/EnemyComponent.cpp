#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/EnemyDef.h"

// define the table exactly once
static const EnemyDef ENEMY_DEFS[] = {
    { EnemyComponent::EnemyType::FireNormal1, 2000.f, 25.f,  10.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::FireNormal2, 2000.f, 25.f,  10.f, 0.5f, 10 },
    { EnemyComponent::EnemyType::FireBoss,   7000.f, 35.f, 70.f, 1.3f, 100 },
    { EnemyComponent::EnemyType::IceNormal1, 2000.f, 25.f,  10.f, 1.3f, 10 },
    { EnemyComponent::EnemyType::IceNormal2, 2000.f, 25.f,  10.f, 1.5f, 10 },
    { EnemyComponent::EnemyType::IceBoss,   7000.f, 35.f, 70.f, 3.5f, 100 },
    { EnemyComponent::EnemyType::ParadiseNormal1, 2000.f, 25.f,  10.f, 1.0f, 10 },
    { EnemyComponent::EnemyType::ParadiseNormal2, 2000.f, 25.f,  10.f, 1.0f, 10 },
    { EnemyComponent::EnemyType::ParadiseBoss,   700.f, 35.f, 70.f, 1.4f, 100 },
    { EnemyComponent::EnemyType::HellNormal1, 2000.f, 25.f,  10.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::HellNormal2, 2000.f, 25.f,  10.f, 0.2f, 10 },
    { EnemyComponent::EnemyType::HellBoss,   7000.f, 35.f, 70.f, 1.0f, 100 }
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

void EnemyComponent::loadStats(DifficultyLevel diff) {
    if (diff == DifficultyLevel::Normal) diff = difficulty;
    const EnemyDef& d = getEnemyDef(type);
    health = d.getHealth(diff);
    speed = d.getSpeed(diff);
    damage = d.getDamage(diff);
    scale = d.scale;
    prize = d.getPrize(diff);
}


