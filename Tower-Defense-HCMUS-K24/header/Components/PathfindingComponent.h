#pragma once
#include <vector>
#include <SFML/System.hpp>

struct PathComponent 
{
    std::vector<sf::Vector2f> waypoints;
    std::size_t currentIndex = 0;
    float speed;
};
