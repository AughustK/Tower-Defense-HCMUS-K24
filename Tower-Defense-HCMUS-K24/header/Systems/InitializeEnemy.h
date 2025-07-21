#pragma once

#include "System.h"
#include "../Managers/World.h"
#include "../Components/EnemyComponent.h"
#include "../Components/PathfindingComponent.h"

class EnemySpawnSystem : public System
{
private:
	std::vector<EntityID> createdEnemies;

public:
	void update(float deltaTime) override;
	void spawnWave(World& world, const std::vector<sf::Vector2f>& path, std::size_t count, const string& map, EnemyComponent::EnemyType t);
	void destroyAllEnemies(World& world);
};