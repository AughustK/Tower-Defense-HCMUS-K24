#pragma once

#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"
#include <functional>
#include <string>

using namespace sf;
using namespace std;

struct ClickComponent 
{
    sf::FloatRect area;                              // in world coords
    std::function<void(EntityID, World&)> onClick;   

    bool tryClick(Vector2f mousePos, EntityID entityId, World& world);
};