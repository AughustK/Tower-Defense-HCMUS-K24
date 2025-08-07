#pragma once
#include <cstdint>
#include <iostream>

using EntityID = uint32_t;

struct CircleComponent
{
	float x, y; 
	float radius; 
	EntityID owner; 
	enum class CollisionType { Enemy, TowerRange, Projectile };
	CollisionType tag;

	CircleComponent() : x(0.0f), y(0.0f), radius(0.0f), owner(0), tag(CollisionType::Projectile) {}

	CircleComponent(float xPos, float yPos, float cRadius, EntityID cOwner, CollisionType cTag) 
		: x(xPos), y(yPos), radius(cRadius), owner(cOwner), tag(cTag) {}
};

inline std::ostream& operator<<(std::ostream& os, const CircleComponent& c) 
{
    os << "CircleComponent\n"
        << c.x << ' '
        << c.y << ' '
        << c.radius << ' '
        << c.owner << ' '
        << static_cast<int>(c.tag) << "\n";
    return os;
}

inline std::istream& operator>>(std::istream& is, CircleComponent& c) 
{
    is >> c.x >> c.y
        >> c.radius
        >> c.owner;

    int tagInt;
    is >> tagInt;
    c.tag = static_cast<CircleComponent::CollisionType>(tagInt);

    return is;
}

 