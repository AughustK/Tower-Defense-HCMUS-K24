#include "../../header/Systems/TextRenderSystem.h"
#include "../../header/Components/TagComponent.h"
#include "../../header/Components/NoDimComponent.h"

void TextRenderSystem::update(float deltaTime)
{

}

void TextRenderSystem::renderGameplay(World& world, bool dimText)
{
    for (EntityID entity : entities)
    {
        auto& textComp = world.getComponent<TextComponent>(entity);
        if (world.hasComponent<NoDimComponent>(entity)) {
            world.window.draw(textComp.txt);
            continue;
        }
        sf::Color c = textComp.txt.getFillColor();
        if (dimText) {
            c.a = 100;
        }
        else {
            c.a = 255;
        }
        textComp.txt.setFillColor(c);
        world.window.draw(textComp.txt);
    }
}

void TextRenderSystem::render(World& world)
{
    for (EntityID entity : entities)
    {
        auto& textComp = world.getComponent<TextComponent>(entity);
        world.window.draw(textComp.txt);
    }
}