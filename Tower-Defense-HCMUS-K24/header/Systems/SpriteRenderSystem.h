#pragma once
#include <iostream>
#include "System.h"
#include "../Managers/World.h"
#include "../Components/UISpriteComponent.h"

using std::cout;

class SpriteRenderSystem : public System 
{
public:
    SpriteRenderSystem() = default;

    void update(float deltaTime) override;
    void updateAnimation(float deltaTime, World &world);
    void render(World& world);
};