#pragma once
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"

using namespace std;
using namespace sf;

struct TextComponent
{
	Text txt;
	std::shared_ptr<sf::Font> font;
	FloatRect bound;
	Color originColor;
	bool isHover;
	Color hoverColor;
	string fontP;

	TextComponent() = default;
	function<void(EntityID, World&)> onClick;
	TextComponent(const string& str, int n, const string& fontPath, Color color, const Vector2f& position, bool hover, sf::Color outline, float thickness, sf::Color hoverC=Color::Cyan);
	bool contains(Vector2f point) const;
	bool tryClick(Vector2f mousePos, EntityID entityId, World& world);
	void tryHover(Vector2f mousePos, EntityID entityId, World& world);
	void setString(string newTxt);
};

inline std::ostream& operator<<(std::ostream& os, const TextComponent& t) {
    os << "TextComponent\n";
    sf::Vector2f origin = t.txt.getOrigin();
    os << "\"" << t.txt.getString().toAnsiString() << "\" "
        << t.txt.getCharacterSize() << " "
        << "\"" << t.fontP << "\" "
        << t.txt.getFillColor().toInteger() << " "
        << t.txt.getPosition().x << " " << t.txt.getPosition().y << " "
        << t.txt.getOutlineColor().toInteger() << " "
        << t.txt.getOutlineThickness() << " "
        << t.isHover << " "
        << t.hoverColor.toInteger() << " "
        << origin.x << " " << origin.y << "\n";
    return os;
}


inline std::istream& operator>>(std::istream& is, TextComponent& t) {
    std::string textStr, fontPath;

    is >> std::ws;
    is.get();  
    std::getline(is, textStr, '"');

    int size;
    is >> size;

    is >> std::ws;
    is.get();  
    std::getline(is, fontPath, '"');

    unsigned int fillColorInt;
    float posX, posY;
    unsigned int outlineColorInt;
    float outlineThickness;
    bool hover;
    unsigned int hoverColorInt;
    float originX, originY;

    is >> fillColorInt
        >> posX >> posY
        >> outlineColorInt >> outlineThickness
        >> hover >> hoverColorInt
        >> originX >> originY;

    t.txt.setString(textStr);
    t.txt.setCharacterSize(size);
    t.fontP = fontPath;

    t.font = std::make_shared<sf::Font>();
    if (!t.font->loadFromFile(fontPath)) {
        std::cerr << "[TextComponent] Failed to load font from: " << fontPath << "\n";
    }
    t.txt.setFont(*t.font);

    t.originColor = sf::Color(fillColorInt);
    t.txt.setFillColor(t.originColor);
    t.txt.setPosition({ posX, posY });
    t.txt.setOutlineColor(sf::Color(outlineColorInt));
    t.txt.setOutlineThickness(outlineThickness);
    t.txt.setOrigin({ originX, originY });

    t.isHover = hover;
    t.hoverColor = sf::Color(hoverColorInt);

    return is;
}





