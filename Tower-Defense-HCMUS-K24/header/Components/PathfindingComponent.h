#pragma once
#include <vector>
#include <SFML/System.hpp>
#include <iostream>

struct PathComponent 
{
    std::vector<sf::Vector2f> waypoints;
    std::size_t currentIndex = 0;
    float speed;
    bool shouldTeleport = true; // New flag to indicate if enemy should teleport to first waypoint
};

inline std::ostream& operator<<(std::ostream& os, const PathComponent& path) 
{
    os << "PathComponent\n";
    os << path.waypoints.size() << "\n";
    for (const auto& point : path.waypoints) {
        os << point.x << " " << point.y << "\n";
    }
    os << path.currentIndex << " " << path.speed << " " << path.shouldTeleport << "\n";
    return os;
}

inline std::istream& operator>>(std::istream& is, PathComponent& path) 
{
    std::size_t count;
    is >> count;
    path.waypoints.clear();
    for (std::size_t i = 0; i < count; ++i) {
        float x, y;
        is >> x >> y;
        path.waypoints.emplace_back(x, y);
    }
    is >> path.currentIndex >> path.speed >> path.shouldTeleport;
    return is;
}