#pragma once
#include <iostream>

struct VelocityComponent 
{
	float x, y;

	VelocityComponent() : x(0.0f), y(0.0f) {}
	VelocityComponent(float xPos, float yPos) : x(xPos), y(yPos) {}
};

inline std::ostream& operator<<(std::ostream& os, const VelocityComponent& v) {
	os << "VelocityComponent\n";
	os << v.x << " " << v.y << "\n";
	return os;
}

inline std::istream& operator>>(std::istream& is, VelocityComponent& v) {
	is >> v.x >> v.y;
	return is;
}
