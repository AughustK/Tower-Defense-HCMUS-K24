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
{
}

void SpriteRenderSystem::clear()
{
    // Clear animation timers and current frames when the system is reset
    animationTimers.clear();
    currentFrames.clear();
    
    // Clear the entities list to ensure no stale references
    entities.clear();
    
    std::cout << "[SpriteRenderSystem] Cleared animation data and entity list\n";
}

void SpriteRenderSystem::removeEntity(EntityID entityID)
{
    // Remove entity from the entities list
    auto it = std::find(entities.begin(), entities.end(), entityID);
    if (it != entities.end()) {
        entities.erase(it);
    }
    
    // Clean up animation data for this entity
    animationTimers.erase(entityID);
    currentFrames.erase(entityID);
    
    std::cout << "[SpriteRenderSystem] Removed entity " << entityID << " from system\n";
}


void SpriteRenderSystem::updateAnimation(float deltaTime, World& world)
{
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

                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / sprite.frameCount;
                        sprite.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, sprite.texture->getSize().y));

                        sf::FloatRect bounds = sprite.sprite.getLocalBounds();
                        sprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                    }
                }

                // Update animation
                this->animationTimers[entity] += deltaTime;

                if (animationTimers[entity] >= sprite.frameRate)
                {
                    this->animationTimers[entity] = 0;
                    this->currentFrames[entity] = (this->currentFrames[entity] + 1) % sprite.frameCount;

                    if (sprite.texture && sprite.texture->getSize().x > 0)
                    {
                        int frameWidth = sprite.texture->getSize().x / sprite.frameCount;
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