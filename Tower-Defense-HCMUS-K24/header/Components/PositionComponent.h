#pragma once
#include <iostream>

struct PositionComponent
{
	float x, y;
	enum class Type { Enemy, Tower, Projectile, Icon };  // Type of position component, using an enum class for better type safety
	Type type;

	PositionComponent() : x(0.0f), y(0.0f), type(Type::Enemy) {}

	PositionComponent(float xPos, float yPos, Type t)
		: x(xPos),
		y(yPos), 
		type(t) 
	{}
};

inline std::ostream& operator<<(std::ostream& os, const PositionComponent& p) {
    os << "PositionComponent\n";
    os << p.x << " " << p.y << " " << static_cast<int>(p.type) << '\n';
    return os;
}

inline std::istream& operator>>(std::istream& is, PositionComponent& p) {
    int typeInt;
    is >> p.x >> p.y >> typeInt;
    p.type = static_cast<PositionComponent::Type>(typeInt);
    return is;
}




