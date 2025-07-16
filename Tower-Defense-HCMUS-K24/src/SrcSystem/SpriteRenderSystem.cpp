#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/EnemyComponent.h"

#include <unordered_map>

void SpriteRenderSystem::render(World& world)
{
    for (EntityID entity : entities) 
    {
        auto& spriteComp = world.getComponent<SpriteComponent>(entity);
        world.window.draw(spriteComp.sprite);
    }
}

void SpriteRenderSystem::update(float deltaTime)
{
    // Keep this empty or add non-World dependent logic
}

void SpriteRenderSystem::updateAnimation(float deltaTime, World& world)
{
    // Static map to store animation state for each entity
    static std::unordered_map<EntityID, float> animationTimers;
    static std::unordered_map<EntityID, int> currentFrames;

    for (auto entity : entities)
    {
        if (world.hasComponent<SpriteComponent>(entity))
        {
            auto& sprite = world.getComponent<SpriteComponent>(entity);

            // Update position from PositionComponent
            if (world.hasComponent<PositionComponent>(entity))
            {
                auto& pos = world.getComponent<PositionComponent>(entity);
                sprite.sprite.setPosition(pos.x, pos.y);
            }

            // Only animate entities with EnemyComponent
            if (world.hasComponent<EnemyComponent>(entity))
            {
                if (animationTimers.find(entity) == animationTimers.end())
                {
                    animationTimers[entity] = 0.0f;
                    currentFrames[entity] = 0;

                    const int FRAME_COUNT = 4;
                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / FRAME_COUNT;
                        sprite.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, sprite.texture->getSize().y));
                    }
                }

                // Update animation
                animationTimers[entity] += deltaTime;
                const float FRAME_TIME = 0.2f;

                if (animationTimers[entity] >= FRAME_TIME)
                {
                    animationTimers[entity] = 0.0f;
                    const int FRAME_COUNT = 4;
                    currentFrames[entity] = (currentFrames[entity] + 1) % FRAME_COUNT;

                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / FRAME_COUNT;
                        sprite.sprite.setTextureRect(sf::IntRect(
                            currentFrames[entity] * frameWidth, 0,
                            frameWidth, sprite.texture->getSize().y
                        ));
                    }
                }
            }
        }
    }
}