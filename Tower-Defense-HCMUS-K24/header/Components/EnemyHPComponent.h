#pragma once
#include <iostream>
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

inline std::ostream& operator<<(std::ostream& os, const EnemyHPComponent& hp) {
    os << "EnemyHPComponent\n";
    os << hp.maxHP << ' '
        << hp.currentHP << ' '
        << hp.border.getSize().x << ' '
        << hp.border.getSize().y << ' '
        << hp.offset.x << ' '
        << hp.offset.y << '\n';
    return os;
}

inline std::istream& operator>>(std::istream& is, EnemyHPComponent& hp) {
    float width, height;
    is >> hp.maxHP >> hp.currentHP >> width >> height >> hp.offset.x >> hp.offset.y;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    hp.init(hp.maxHP, hp.currentHP, width, height, hp.offset);
    return is;
}

