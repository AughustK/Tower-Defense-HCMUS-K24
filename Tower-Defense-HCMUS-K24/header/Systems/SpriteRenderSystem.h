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

    void update(float deltaTime) {}
    void render(World& world);
};