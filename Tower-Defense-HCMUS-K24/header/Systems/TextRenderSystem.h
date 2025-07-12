#pragma once
#include <iostream>
#include "System.h"
#include "../Managers/World.h"
#include "../Components/UITextComponent.h"

using std::cout;

class TextRenderSystem : public System 
{
public:
    TextRenderSystem() = default;

    void update(float deltaTime) override;
    void render(World& world);
};