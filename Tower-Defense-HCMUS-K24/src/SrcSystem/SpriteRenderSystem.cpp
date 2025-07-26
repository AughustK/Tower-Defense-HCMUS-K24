#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/EnemyComponent.h"
#include <iostream>

void SpriteRenderSystem::render(World& world)
{
    for (EntityID entity : entities)
    {
        auto& spriteComp = world.getComponent<SpriteComponent>(entity);
        //Only set position if the entity has a PositionComponent
        if (world.hasComponent<PositionComponent>(entity)) {
            auto& pos = world.getComponent<PositionComponent>(entity);
            spriteComp.sprite.setPosition(pos.x, pos.y);
        }
        world.window.draw(spriteComp.sprite);
    }
}


void SpriteRenderSystem::update(float deltaTime) 
{}


void SpriteRenderSystem::updateAnimation(float deltaTime, World& world)
{
    // Static map to store animation state for each entity
    // static std::unordered_map<EntityID, float> animationTimers;
    // static std::unordered_map<EntityID, int> currentFrames;

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
                if (this->animationTimers.find(entity) == this->animationTimers.end())
                {
                    this->animationTimers[entity] = 0.0f;
                    this->currentFrames[entity] = 0;

                    const int FRAME_COUNT = 20;
                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / FRAME_COUNT;
                        sprite.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, sprite.texture->getSize().y));

                        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
                        sprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                    }
                }

                // Update animation
                this->animationTimers[entity] += deltaTime;
                const float FRAME_TIME = 0.05f;

                if (animationTimers[entity] >= FRAME_TIME)
                {
                    this->animationTimers[entity] = 0;
                    const int FRAME_COUNT = 20;
                    this->currentFrames[entity] = (this->currentFrames[entity] + 1) % FRAME_COUNT;

                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / FRAME_COUNT;
                        sprite.sprite.setTextureRect(sf::IntRect(
                            this->currentFrames[entity] * frameWidth, 0,
                            frameWidth, sprite.texture->getSize().y
                        ));
                    }

                    sf::FloatRect bounds = sprite.sprite.getLocalBounds();
                    sprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                }
            }
        }
    }
}