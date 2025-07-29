#include "../../header/Components/CastleHPComponent.h"
#include <algorithm>

CastleHPComponent::CastleHPComponent() = default;

CastleHPComponent::CastleHPComponent(int maxHP, int currentHP, float x, float y, float width, float height)
    : maxHP(maxHP), currentHP(currentHP)
{
    font = std::make_shared<sf::Font>();
    font->loadFromFile("assets/Font/Minecraft-Regular.otf");

    border.setSize({ width, height });
    border.setPosition(x, y);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(2.f);

    // Fill (thanh máu)
    fill.setSize({ width, height });
    fill.setPosition(x, y);
    fill.setFillColor(sf::Color::Green); 

    // Text "HP: xx"
    hpText.setFont(*font);
    hpText.setCharacterSize(25);
    hpText.setFillColor(sf::Color::White);
    hpText.setOutlineThickness(2.f);
    hpText.setString("HP: " + std::to_string(currentHP));
    hpText.setPosition(x, y - 35.f); 
}

void CastleHPComponent::setHP(int hp)
{
    currentHP = std::max(0, std::min(hp, maxHP));

    float percent = static_cast<float>(currentHP) / maxHP;
    float fullWidth = border.getSize().x;
    fill.setSize({ fullWidth * percent, border.getSize().y });

    if (percent <= 0.2f)
        fill.setFillColor(sf::Color::Red);
    else if (percent <= 0.75f)
        fill.setFillColor(sf::Color::Yellow);
    else
        fill.setFillColor(sf::Color::Green);

    hpText.setString("HP: " + std::to_string(currentHP));
}
