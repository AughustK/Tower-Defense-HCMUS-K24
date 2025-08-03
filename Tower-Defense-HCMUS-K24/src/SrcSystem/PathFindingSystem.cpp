
#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Components/PathfindingComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/EnemyDef.h"
#include "../../header/Components/CircleComponent.h"
#include <iostream>


void PathFollowingSystem::update(float deltaTime, World& world)
{
    for (auto entity : entities)
    {
        auto& pathComponent = world.getComponent<PathComponent>(entity);
        auto& positionComponent = world.getComponent<PositionComponent>(entity);
        auto& velComponent = world.getComponent<VelocityComponent>(entity);

        // Handle teleportation to first waypoint if needed
        if (pathComponent.shouldTeleport && !pathComponent.waypoints.empty())
        {   
            // Teleport to first waypoint
            positionComponent.x = pathComponent.waypoints[0].x;
            positionComponent.y = pathComponent.waypoints[0].y;
            
            // Update collision circle position
            if (world.hasComponent<CircleComponent>(entity)) {
                auto& circleComp = world.getComponent<CircleComponent>(entity);
                circleComp.x = positionComponent.x;
                circleComp.y = positionComponent.y;
            }
            
            // Update sprite position
            if (world.hasComponent<SpriteComponent>(entity)) {
                auto& spriteComp = world.getComponent<SpriteComponent>(entity);
                spriteComp.sprite.setPosition(positionComponent.x, positionComponent.y);
            }
            
            // Mark teleportation as complete
            pathComponent.shouldTeleport = false;
            continue; // Skip normal movement this frame
        }

        // If there are waypoints left
        if (pathComponent.currentIndex < pathComponent.waypoints.size())
        {
            sf::Vector2f target = pathComponent.waypoints[pathComponent.currentIndex];
            sf::Vector2f currentPos = { positionComponent.x, positionComponent.y };
            sf::Vector2f direction = target - currentPos;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < 4.f)
            {
                pathComponent.currentIndex++;
            }
            else
            {
                sf::Vector2f direction = target - currentPos;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                if (distance < 4.f) {
                    pathComponent.currentIndex++;
                }
                else {
                    // compute desired velocity
                    sf::Vector2f desiredVel = (direction / distance) * pathComponent.speed;

                    // ease actual velocity toward desired
                    const float accel = 10.0f;
                    velComponent.x += (desiredVel.x - velComponent.x) * accel * deltaTime;
                    velComponent.y += (desiredVel.y - velComponent.y) * accel * deltaTime;

                    // move by smoothed velocity
                    positionComponent.x += velComponent.x * deltaTime;
                    positionComponent.y += velComponent.y * deltaTime;

                    // update circle & sprite positions exactly once
                    if (world.hasComponent<CircleComponent>(entity)) {
                        auto& circleComp = world.getComponent<CircleComponent>(entity);
                        circleComp.x = positionComponent.x;
                        circleComp.y = positionComponent.y;
                    }
                    if (world.hasComponent<SpriteComponent>(entity)) {
                        auto& spriteComp = world.getComponent<SpriteComponent>(entity);
                        // optional: flip based on velComponent.x
                        float baseScale = EnemyComponent::getEnemyDef(
                            world.getComponent<EnemyComponent>(entity).type
                        ).scale;
                        spriteComp.sprite.setScale(
                            (velComponent.x < -0.1f ? -baseScale : baseScale),
                            baseScale
                        );
                        spriteComp.sprite.setPosition(positionComponent.x, positionComponent.y);
                    }
                }
            }
        }
        else
        {
            velComponent.x = velComponent.y = 0.f;
        }
    }
}

