#pragma once

#include "System.h"
#include "../Managers/EntityManager.h"
#include "../Managers/ComponentManager.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CircleComponent.h"
#include "../Components/TowerComponent.h"

#include <vector>
#include <cmath>

class TowerSystem : public System
{
private:
	EntityManager& entityManager;
	ComponentManager& componentManager;
	std::vector<EntityID> projectilePool;
	long long nextProjectile = 0;

public:
	TowerSystem(EntityManager& em, ComponentManager& cm)
				: entityManager(em), componentManager(cm) {};
	void initializePool(long long poolSize);
	void update(float deltaTime,
		ComponentArray<TowerComponent>& towerArray,
		ComponentArray<CircleComponent>& circleArray,
		ComponentArray<ProjectileComponent>& projectileArray,
		ComponentArray<VelocityComponent>& velocityArray,
		ComponentArray<PositionComponent>& positionComponent);
};