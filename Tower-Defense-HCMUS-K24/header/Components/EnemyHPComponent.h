#pragma once
#include <SFML/Graphics.hpp>

struct EnemyHPComponent {
    int maxHP;
    int currentHP;

    sf::RectangleShape border;
    sf::RectangleShape fill;

    sf::Vector2f offset; 

    EnemyHPComponent();
    EnemyHPComponent(int maxHP, int currentHP, float width, float height, sf::Vector2f offset);

    void setHP(int hp);
    void updatePosition(const sf::Vector2f& basePos);
    void init(int maxHP, int currentHP, float width, float height, sf::Vector2f offset);
};
