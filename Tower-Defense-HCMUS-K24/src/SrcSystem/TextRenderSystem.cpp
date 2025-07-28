#include "../../header/Systems/TextRenderSystem.h"

void TextRenderSystem::update(float deltaTime)
{

}

void TextRenderSystem::render(World& world)
{
    for (EntityID entity : entities) 
    {
        auto& textComp = world.getComponent<TextComponent>(entity);
        world.window.draw(textComp.txt);
    }
}