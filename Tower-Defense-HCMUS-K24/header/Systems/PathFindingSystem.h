#pragma once

#include "System.h"
#include <SFML/System/Vector2.hpp>
#include "../Managers/World.h"


class PathFollowingSystem : public System 
{
private:
    float threshold;

public:
    // reachThreshold: distance (in px) at which we consider a waypoint reached
    PathFollowingSystem(float reachThreshold = 0.05f) : threshold(reachThreshold) {}

    void update(float deltaTime) {}
    void update(float deltaTme, World& world);
};

