#pragma once

#include "ComponentArray.h"          
#include "../Components/CircleComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TowerComponent.h"
#include "../Components/VelocityComponent.h"
#include "EntityManager.h"

#include <vector>
#include <cmath>

using std::vector;

class CollisionSystem
{
public:
	float computeDistanceOfTwoPoint(float dX, float dY) const;
	void updateCheck(ComponentArray<VelocityComponent>& velocityArray,
					 ComponentArray<CircleComponent>& circleArray,
					 ComponentArray<TowerComponent>& towerRangeArray,
					 ComponentArray<HealthComponent>& healthArray,
					 EntityManager& entityManager);
};