#pragma once
#include <iostream>
#include "../System.h"
#include "../Managers/World.h"
#include "../UIComponents/UISpriteComponent.h"

using std::cout;

class SpriteRenderSystem : public System {

public:
    SpriteRenderSystem() = default;

    void update(float deltaTime) override {
        
    }

    void render(World& world) {
        //std::cout << "Entities in SpriteRenderSystem: " << entities.size() << "\n";
        for (EntityID entity : entities) {
            //std::cout << "Drawing entity: " << entity << "\n";
            auto& spriteComp = world.getComponent<SpriteComponent>(entity);
            world.window.draw(spriteComp.sprite);
        }
    }
};
