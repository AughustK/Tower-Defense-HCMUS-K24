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

    std::unordered_map<EntityID, float> animationTimers;
    std::unordered_map<EntityID, int> currentFrames;

    void update(float deltaTime);
    void render(World& world);
    void updateAnimation(float deltaTime, World& world);
    void clear();
    void removeEntity(EntityID entityID) override;
};