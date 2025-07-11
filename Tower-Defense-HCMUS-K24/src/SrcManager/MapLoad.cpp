#include "../../header/Managers/MapLoad.h"
#include <fstream>
#include <sstream>
#include <iostream> 

bool MapLoader::loadFromFile(const std::string& filename, std::vector<sf::Vector2f>& outWaypoints)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "File not open" << '\n';
        return false;
    }

    outWaypoints.clear();
    std::string line;

    while (std::getline(file, line)) 
    {
        // strip leading whitespace
        size_t first = line.find_first_not_of(" \t");
        if (first == std::string::npos || line[first] == '#')
            continue;               // skip blank/comment

        std::istringstream iss(line);
        float x, y;
        if (!(iss >> x >> y))
            return false;           // malformed line
        outWaypoints.emplace_back(x, y);
    }

    return true;
}