#pragma once
#include "../Tower-Defense-HCMUS-K24/header/Managers/GameState.h"
#include "../Tower-Defense-HCMUS-K24/GamePlay.h"
#include "../Tower-Defense-HCMUS-K24/header/UISystems/SpriteRenderSystem.h"
#include "../Tower-Defense-HCMUS-K24/header/Managers/World.h"
#include <iostream>

class SpriteRenderSystem;

class MainMenu : public GameState {
public:
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
    void onEnter(World &world) override;
    void onExit(World &world) override;
};

void MainMenu::handleEvent(World& world, sf::Event& event) {

}

void MainMenu::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void MainMenu::onEnter(World& world)
{
    std::cout << "[MainMenu] onEnter called\n";
    EntityID background = world.createEntity();

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/Bg/MenuV2-1.png")) {
        std::cerr << "Failed to load background texture\n";
        return;
    }
    static sf::Texture persistentBackgroundTexture = backgroundTexture;

    SpriteComponent spriteComp(persistentBackgroundTexture, { 0.f, 0.f });

    world.addComponent(background, spriteComp);
}

void MainMenu::onExit(World& world) {

}

void MainMenu::update(World& world, float dt) {
    
}

