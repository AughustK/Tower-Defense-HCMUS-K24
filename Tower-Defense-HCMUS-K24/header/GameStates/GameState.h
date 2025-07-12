#pragma once
#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"

class World;

class GameState
{
protected:
    std::vector<EntityID> createdEntities;
public:
    virtual ~GameState() = default;
    void registerEntity(EntityID id) 
    {
        createdEntities.push_back(id);
    }
    virtual void handleEvent(World& world, sf::Event& event) = 0;
    virtual void update(World& world, float dt) = 0;
    virtual void render(World& world, sf::RenderWindow& window) = 0;
    virtual void onEnter(World& world);
    virtual void onExit(World& world);
};