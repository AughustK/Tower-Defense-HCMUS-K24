#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/EnemyComponent.h"
#include "../../header/Components/TowerComponent.h"
#include <iostream>

void SpriteRenderSystem::render(World& world)
{
    for (EntityID entity : entities)
    {
        if (!world.hasComponent<SpriteComponent>(entity)) {
            continue;
        }
        
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

    entities.clear();
    std::cout << "[SpriteRenderSystem] Cleared animation data\n";
}

void SpriteRenderSystem::hideEntity(EntityID entityID)
{
    // Clean up animation data for hidden entities to prevent memory leaks
    animationTimers.erase(entityID);
    currentFrames.erase(entityID);
}

void SpriteRenderSystem::showEntity(EntityID entityID)
{
    animationTimers[entityID] = 0.f;
    currentFrames[entityID] = 0;
}

void SpriteRenderSystem::addEntityToSystem(EntityID entityID)
{
    entities.push_back(entityID);
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

            if (world.hasComponent<EnemyComponent>(entity) ||
                world.hasComponent<TowerComponent>(entity))
            {
                auto& sprite = world.getComponent<SpriteComponent>(entity);

                // *** EARLY OUT IF NO TEXTURE ***
                if (!sprite.texture)
                    continue;

                // find or create your timer & frame entries
                float& timer = animationTimers[entity];        
                int& frame = currentFrames[entity];          

                // sanity
                int   count = std::max(1, sprite.frameCount);
                float rate = std::max(0.0001f, sprite.frameRate);

                // grab the texture size once
                auto sz = sprite.texture->getSize();
                int  frameW = sz.x / count;

                // ON FIRST REGISTRATION: set frame 0
                if (timer == 0.0f && frame == 0)
                {
                    sprite.sprite.setTextureRect(sf::IntRect(static_cast<int>(0), static_cast<int>(0), static_cast<int>(frameW), static_cast<int>(sz.y)));
                    auto b = sprite.sprite.getLocalBounds();
                    sprite.sprite.setOrigin(b.width / 2.f, b.height / 2.f);
                }

                // accumulate time
                timer += deltaTime;

                // advance as many frames as needed
                while (timer >= rate) {
                    timer -= rate;
                    frame = (frame + 1) % count;

                    sprite.sprite.setTextureRect(sf::IntRect(static_cast<int>(frame) * static_cast<int>(frameW), 0, static_cast<int>(frameW), static_cast<int>(sz.y)));
                    auto b = sprite.sprite.getLocalBounds();
                    sprite.sprite.setOrigin(b.width / 2.f, b.height / 2.f);
                }
            }
        }
    }
}