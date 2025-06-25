#pragma once
#include <SFML/Graphics.hpp>

class World;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleEvent(World& world, sf::Event& event) = 0;
    virtual void update(World& world, float dt) = 0;
    virtual void render(World& world, sf::RenderWindow& window) = 0;
    virtual void onEnter(World& world);
    virtual void onExit(World& world);
};