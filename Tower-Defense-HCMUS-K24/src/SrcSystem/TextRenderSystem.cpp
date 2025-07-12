#include "../../header/Systems/TextRenderSystem.h"

void TextRenderSystem::update(float deltaTime)
{

}

void TextRenderSystem::render(World& world)
{
    //std::cout << "Entities in SpriteRenderSystem: " << entities.size() << "\n";
    for (EntityID entity : entities) 
    {
        //std::cout << "Drawing entity: " << entity << "\n";
        auto& textComp = world.getComponent<TextComponent>(entity);
        world.window.draw(textComp.txt);
    }
}