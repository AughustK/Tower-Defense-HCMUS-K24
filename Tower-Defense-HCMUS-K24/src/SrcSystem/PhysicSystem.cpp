#include "../../header/Systems/PhysicSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"

void PhysicSystem::update(float deltaTime, World& world)
{
	auto& posArr = world.getComponentArray<PositionComponent>();
	auto& velArr = world.getComponentArray<VelocityComponent>();
	update(deltaTime, posArr, velArr);
}

void PhysicSystem::update(float deltaTime, ComponentArray<PositionComponent>& positionArray, ComponentArray<VelocityComponent>& velocityArray)
{
	for (auto entity : entities)
	{
		if (!positionArray.containData(entity) || !velocityArray.containData(entity))
		{
			continue;
		}

		auto& pos = positionArray.getData(entity);
		auto& velocity = velocityArray.getData(entity);

		pos.x += velocity.x * deltaTime;
		pos.y += velocity.y * deltaTime;
	}
}
