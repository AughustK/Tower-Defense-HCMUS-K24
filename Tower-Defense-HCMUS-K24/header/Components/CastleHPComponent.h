#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <iostream>

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

inline std::ostream& operator<<(std::ostream& os, const CastleHPComponent& c) 
{
    os << "CastleHPComponent\n";
    os << c.maxHP << " " << c.currentHP << " ";
    os << c.fill.getPosition().x << " " << c.fill.getPosition().y << " ";
    os << c.fill.getSize().x << " " << c.fill.getSize().y << " ";
    os << c.fill.getFillColor().toInteger() << " ";
    os << c.border.getOutlineColor().toInteger() << " " << c.border.getOutlineThickness() << " ";

    os << "\"" << c.hpText.getString().toAnsiString() << "\" "
        << c.hpText.getCharacterSize() << " "
        << c.hpText.getFillColor().toInteger() << " "
        << c.hpText.getPosition().x << " " << c.hpText.getPosition().y << " "
        << c.hpText.getOutlineColor().toInteger() << " "
        << c.hpText.getOutlineThickness() << "\n";

    return os;
}

inline std::istream& operator>>(std::istream& is, CastleHPComponent& c) 
{
    float posX, posY, width, height;
    unsigned int fillColor, borderColor;
    float borderThickness;

    std::string hpStr;
    char quote;
    int charSize;
    unsigned int textColor, outlineColor;
    float textPosX, textPosY, outlineThickness;

    is >> c.maxHP >> c.currentHP
        >> posX >> posY >> width >> height
        >> fillColor >> borderColor >> borderThickness;

    is >> std::ws >> quote;
    std::getline(is, hpStr, '"');

    is >> charSize
        >> textColor >> textPosX >> textPosY >> outlineColor >> outlineThickness;

    // Rectangle fill
    c.fill.setPosition({ posX, posY });
    c.fill.setSize({ width, height });
    c.fill.setFillColor(sf::Color(fillColor));

    // Rectangle border
    c.border.setPosition({ posX, posY });
    c.border.setSize({ width, height });
    c.border.setFillColor(sf::Color::Transparent);
    c.border.setOutlineColor(sf::Color(borderColor));
    c.border.setOutlineThickness(borderThickness);

    constexpr const char* DEFAULT_CASTLE_FONT_PATH = "assets/Font/Minecraft-Regular.otf";
    c.font = std::make_shared<sf::Font>();
    if (!c.font->loadFromFile(DEFAULT_CASTLE_FONT_PATH)) {
        std::cerr << "[CastleHPComponent] Failed to load font from: " << DEFAULT_CASTLE_FONT_PATH << "\n";
    }

    // Text
    c.hpText.setString(hpStr);
    c.hpText.setFont(*c.font);
    c.hpText.setCharacterSize(charSize);
    c.hpText.setFillColor(sf::Color(textColor));
    c.hpText.setPosition({ textPosX, textPosY });
    c.hpText.setOutlineColor(sf::Color(outlineColor));
    c.hpText.setOutlineThickness(outlineThickness);

    return is;
}
