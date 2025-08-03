#pragma once
#include <cstdint>
using EntityID = uint32_t;

struct ProjectileComponent
{
    enum class ProjectileType { Arrow, MagicCircle, CannonBall };

    float x, y;
    float radius;

    ProjectileType tag;
    EntityID owner;

    // Only used by arrows: for OBB
    float arrowLength;

	bool isActive = true; 

    ProjectileComponent()
        : x(0.0f), y(0.0f), radius(0.0f),
        tag(ProjectileType::Arrow),
        owner(0),
        arrowLength(0.0f),
		isActive(true)
    {}

    ProjectileComponent(float xPos, float yPos, float r, ProjectileType type, EntityID cOwner, float length = 0.0f, bool active = true)
        : x(xPos), y(yPos), radius(r),
		tag(type), owner(cOwner), arrowLength(length), isActive(active)
    {}
};