#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

struct CastleHPComponent
{
    int maxHP;
    int currentHP;
    sf::RectangleShape border;
    sf::RectangleShape fill;
    sf::Text hpText;
    std::shared_ptr<sf::Font> font;

    CastleHPComponent();
    CastleHPComponent(int maxHP, int currentHP, float x, float y, float width, float height);

    void setHP(int hp);
};