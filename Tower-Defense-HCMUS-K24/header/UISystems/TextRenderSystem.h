#pragma once
#include <iostream>
#include "../System.h"
#include "../Managers/World.h"
#include "../UIComponents/UITextComponent.h"

using std::cout;

class TextRenderSystem : public System {

public:
    TextRenderSystem() = default;

    void update(float deltaTime) override {

    }

    void render(World& world) {
        //std::cout << "Entities in SpriteRenderSystem: " << entities.size() << "\n";
        for (EntityID entity : entities) {
            //std::cout << "Drawing entity: " << entity << "\n";
            auto& textComp = world.getComponent<TextComponent>(entity);
            world.window.draw(textComp.txt);
        }
    }
};