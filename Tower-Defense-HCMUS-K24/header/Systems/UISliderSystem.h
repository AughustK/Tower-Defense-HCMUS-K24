#pragma once
#include <iostream>
#include "System.h"
#include "../Managers/World.h"
#include "../Components/UISpriteComponent.h"

using std::cout;

class SliderSystem : public System
{
public:
    SliderSystem() = default;

    void update(float deltaTime) override;
    void render(World& world);
    void handleEvent(World& world);
};
