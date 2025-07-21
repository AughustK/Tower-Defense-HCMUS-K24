#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PositionComponent.h"
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


