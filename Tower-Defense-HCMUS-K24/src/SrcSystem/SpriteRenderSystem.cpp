#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Managers/World.h"

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

}