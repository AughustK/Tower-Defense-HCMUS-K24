#pragma once

#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

// BFS-based grid pathfinding. Assumes grid is a 1D array of ints:
// 0 = walk, non-zero = blocked.
// width, height = dimensions of the grid.
// start, goal = cell coordinates in [0,width) x [0,height).
// Returns a vector of cell positions from start to goal (inclusive), or empty if no path.
std::vector<sf::Vector2i> bfsPathfinding(const std::vector<int>& grid, int width, int height, const sf::Vector2i& start, const sf::Vector2i& goal)
{
    auto inBounds = [&](const sf::Vector2i& p) 
    {
        return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
    };
    auto isWalkable = [&](const sf::Vector2i& p) 
    {
        return grid[p.y * width + p.x] == 0;
    };

    std::queue<sf::Vector2i> frontier;
    frontier.push(start);

    // Map from cell key to predecessor key
    std::unordered_map<int, int> cameFrom;
    auto key = [&](const sf::Vector2i& p) { return p.y * width + p.x; };
    cameFrom[key(start)] = -1;

    const sf::Vector2i dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    bool found = false;

    while (!frontier.empty()) 
    {
        sf::Vector2i current = frontier.front();
        frontier.pop();

        if (current == goal) 
        {
            found = true;
            break;
        }

        for (const auto& dir : dirs) 
        {
            sf::Vector2i next = current + dir;
            int nextKey = key(next);
            if (inBounds(next) && isWalkable(next) && !cameFrom.count(nextKey)) 
            {
                frontier.push(next);
                cameFrom[nextKey] = key(current);
            }
        }
    }

    std::vector<sf::Vector2i> path;
    if (!found) return path;

    // Reconstruct path
    int curr = key(goal);
    while (curr != -1) 
    {
        int x = curr % width;
        int y = curr / width;
        path.emplace_back(x, y);
        curr = cameFrom[curr];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
