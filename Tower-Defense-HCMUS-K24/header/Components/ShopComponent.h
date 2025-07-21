#pragma once
#include "SFML/Graphics.hpp"
#include "TowerComponent.h"
#include "UISpriteComponent.h"

struct TowerIconComponent
{
    TowerComponent::TowerType  type;       // Archer, Mage, Cannon
    int        level;      // 0 = base, 1 = upgraded
    SpriteComponent sprite;    // the icon to draw
    sf::FloatRect bounds;  // for hit-testing
};