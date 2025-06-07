#include "PhysicSystem.h"

void PhysicSystem::Update(float deltaTime, ComponentArray<PositionComponent>& posArr, ComponentArray<VelocityComponent>& velArr)
{
	for(auto const & [entity, _] : velArr.getEntityToIndexMap())
	{
		if (!posArr.containData(entity))
		{
			continue;
		}

		auto& pos = posArr.getData(entity);
		auto& vel = velArr.getData(entity);
		pos.x += vel.x * deltaTime;
		pos.y += vel.y * deltaTime;
	}
}
