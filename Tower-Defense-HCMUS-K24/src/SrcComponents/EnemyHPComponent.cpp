#include "../../header/Components/EnemyHPComponent.h"
#include <algorithm>

EnemyHPComponent::EnemyHPComponent() = default;

EnemyHPComponent::EnemyHPComponent(int maxHP, int currentHP, float width, float height, sf::Vector2f offset)
    : maxHP(maxHP), currentHP(currentHP), offset(offset)
{
    border.setSize({ width, height });
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(1.f);

    fill.setSize({ width, height });
    fill.setFillColor(sf::Color::Green);
}

void EnemyHPComponent::setHP(int hp)
{
    currentHP = std::max(0, std::min(hp, maxHP));
    float percent = static_cast<float>(currentHP) / maxHP;

    float fullWidth = border.getSize().x;
    fill.setSize({ fullWidth * percent, border.getSize().y });

    if (percent <= 0.25f)
        fill.setFillColor(sf::Color::Red);
    else if (percent <= 0.75f)
        fill.setFillColor(sf::Color::Yellow);
    else
        fill.setFillColor(sf::Color::Green);
}

void EnemyHPComponent::updatePosition(const sf::Vector2f& basePos)
{
    sf::Vector2f finalPos = basePos + offset;
    border.setPosition(finalPos);
    fill.setPosition(finalPos);
}

void EnemyHPComponent::init(int maxHP, int currentHP, float width, float height, sf::Vector2f offset)
{
    this->maxHP = maxHP;
    this->currentHP = currentHP;
    this->offset = offset;

    // Border setup
    border.setSize({ width, height });
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);

    // Fill setup
    fill.setSize({ width, height });
    fill.setFillColor(sf::Color::Green);
}
