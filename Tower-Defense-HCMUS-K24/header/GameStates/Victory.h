#pragma once
#include "GameState.h"
#include "GamePlay.h"
#include "../Systems/SpriteRenderSystem.h"
#include "../Managers/World.h"
#include "../Components/UITextComponent.h"
#include "../Systems/TextRenderSystem.h"
#include <iostream>
#include <string>

using std::string;

class SpriteRenderSystem;

class Victory : public GameState
{
public:
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
    void onEnter(World& world) override;
    void onExit(World& world) override;
};