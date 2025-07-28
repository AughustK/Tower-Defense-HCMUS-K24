#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include "../Managers/EntityManager.h"  // for EntityID, INVALID_ENTITY
using std::string;

struct EnemyDef;  // forward–decl

struct EnemyComponent {
    enum class EnemyType : uint8_t { FireNormal = 0, FireBoss,
                                     IceNormal, IceBoss,
                                     HellNormal, HellBoss,
                                     ParadiseNormal, ParadiseBoss};
    float x = 0, y = 0;
    float health = 0, speed = 0, damage = 0, scale = 0, prize = 0;
    uint32_t pathIndex = 0;
    EnemyType type = EnemyType::FireNormal;
    EntityID target = INVALID_ENTITY;

    EnemyComponent(float tX = 0, float tY = 0, EnemyType t = EnemyType::FireNormal)
        : x(tX), y(tY), type(t)
    {
        loadStats();
    }

    void loadStats();  

    static const EnemyDef& getEnemyDef(EnemyType t); 
    static std::string getAnimationPath(EnemyType t, const string& map);
};
