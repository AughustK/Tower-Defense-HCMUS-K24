#pragma once
#include <string>
#include <vector>
#include <SFML/System.hpp>

class MapLoader
{
public:
    /// Loads waypoints (in pixels) from a text file into out.
    /// Returns false if file open or parse fails.
    static bool loadFromFile(const std::string& filename, std::vector<sf::Vector2f>& outWaypoints);
};