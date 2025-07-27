#include "../../header/Components/HealthComponent.h"
#include "iostream"
using std::max;

void HealthComponent::takeDamage(int damageAmount)
{
	currentHealth = max(currentHealth - damageAmount, 0);
	std::cout << "[Health] Took " << damageAmount << ", HP now: " << currentHealth << std::endl;
}

bool HealthComponent::isDead() const
{
	return (currentHealth <= 0);
}
