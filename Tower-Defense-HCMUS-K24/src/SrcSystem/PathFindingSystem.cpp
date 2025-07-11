#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Components/PathfindingComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"


void PathFollowingSystem::update(float deltaTme, World& world)
{
    for (auto entity : entities) 
    {
        auto& pathComponent = world.getComponent<PathComponent>(entity);
        auto& positionComponent = world.getComponent<PositionComponent>(entity);
        auto& velComponent = world.getComponent<VelocityComponent>(entity);

        if (pathComponent.currentIndex < pathComponent.waypoints.size()) 
        {
            sf::Vector2f target = pathComponent.waypoints[pathComponent.currentIndex];
            sf::Vector2f dir = target - sf::Vector2f{ positionComponent.x, positionComponent.y };
            float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (distance < threshold) 
            {
                ++pathComponent.currentIndex;
                velComponent.x = velComponent.y = 0.f;
            }
            else {
                float speed = pathComponent.speed; // ensure PathComponent has a speed member
                velComponent.x = (dir.x / distance) * speed;
                velComponent.y = (dir.y / distance) * speed;
            }
        }
        else {
            // reached final waypoint
            velComponent.x = velComponent.y = 0.f;
            // optionally: mark entity as finished or trigger an event
        }
    }
}
