#pragma once

#include <algorithm>
#include <iostream>


struct HealthComponent
{
    int currentHealth;
    int maxHealth;

	HealthComponent() : currentHealth(100), maxHealth(100) {} 

    HealthComponent(int maxHP) : currentHealth(maxHP), maxHealth(maxHP) {}

    void takeDamage(int damageAmount);
    bool isDead() const;
};

inline std::ostream& operator<<(std::ostream& os, const HealthComponent& h) {
    os << "HealthComponent\n";
    os << h.currentHealth << " " << h.maxHealth << "\n";
    return os;
}

inline std::istream& operator>>(std::istream& is, HealthComponent& h) {
    is >> h.currentHealth >> h.maxHealth;
    return is;
}



