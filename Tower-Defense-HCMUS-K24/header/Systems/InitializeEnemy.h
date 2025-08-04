#pragma once

#include "System.h"
#include "../Managers/World.h"
#include "../Components/EnemyComponent.h"
#include "../Components/PathfindingComponent.h"
#include "../Components/EnemyDef.h"

class EnemySpawnSystem : public System 
{
private:
    std::vector<EntityID> activeEnemies;
    std::vector<EntityID> enemyPool;
    std::size_t nextPoolIndex = 0;

public:
    void update(float deltaTime) override;

    // Pool management
    void initPool(World& world, std::size_t count);
    EntityID spawnFromPool(World& world, const std::vector<sf::Vector2f>& path, const string& map, 
                          EnemyComponent::EnemyType t, DifficultyLevel difficulty);
    void returnToPool(World& world, EntityID enemyID);
    void clearPool(World& world);
    int getRemainEnemy() const;
    // Legacy methods (for backward compatibility)
    void spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t, DifficultyLevel difficulty);
    //void destroyAllEnemies(World& world);
    std::vector<EntityID> getActiveEnemies();
};