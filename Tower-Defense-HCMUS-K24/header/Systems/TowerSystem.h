#pragma once

#include "System.h"
#include "../Managers/EntityManager.h"
#include "../Managers/ComponentManager.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CircleComponent.h"
#include "../Components/TowerComponent.h"
#include "../Managers/World.h"

#include <vector>
#include <cmath>

class TowerSystem : public System
{
private:
	std::vector<EntityID> projectilePool;
	long long nextProjectile = 0;

public:
	TowerSystem() = default;
	void initializePool(long long poolSize, World& world);
	void update(float deltaTime, World& world);
	void update(float dt) override;
};