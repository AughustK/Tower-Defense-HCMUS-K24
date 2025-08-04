#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"

using namespace sf;
using namespace std;

struct SliderComponent
{
    float min = 0.f;
    float max = 100.f;
    float value;
    bool isDragging = false;
    string type;

    sf::RectangleShape track, fill, handle;

    sf::Text label;
    std::shared_ptr<sf::Font> font;
    sf::Text valueText;

    SliderComponent() = default;
    SliderComponent(float width, float x, float y, const std::string& labelText, float vol)
        : type(labelText), min(0.f), max(100.f), isDragging(false), value(vol)
    {
        font = make_shared<Font>();
        font->loadFromFile("assets/Font/Minecraft-Regular.otf");

        // Track
        track.setSize({ width, 20.f });
        track.setPosition(x, y);
        track.setFillColor(sf::Color(60, 60, 60, 200));
        track.setOutlineColor(sf::Color::White);
        track.setOutlineThickness(1.f);

        // Fill
        fill.setSize({ width, 20.f });
        fill.setPosition(x, y);
        fill.setFillColor(sf::Color(100, 200, 250, 220));

        // Handle
        handle.setSize({ 40.f, 40.f });
        handle.setOrigin(handle.getSize().x / 2, handle.getSize().y / 2);
        handle.setFillColor(sf::Color::White);

        // Label
        label.setFont(*font);
        label.setString(labelText);
        label.setCharacterSize(40);
        label.setFillColor(sf::Color::White);
        label.setOutlineThickness(2.f);

        // Get local bounds
        FloatRect labelBounds = label.getLocalBounds();
        label.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
        label.setPosition(x + track.getSize().x / 2.f - 40.f, y - 50.f); // l?ch trái chút n?u c?n ch? cho %

        // Value text
        valueText.setFont(*font);
        valueText.setCharacterSize(40);
        valueText.setFillColor(sf::Color::White);
        valueText.setString(to_string(static_cast<int>(value)) + "%");

        FloatRect valueBounds = valueText.getLocalBounds();
        valueText.setOrigin(valueBounds.width / 2.f, valueBounds.height / 2.f);
        FloatRect labelGlobal = label.getGlobalBounds();
        valueText.setPosition(
            labelGlobal.left + labelGlobal.width + 70.f,  
            label.getPosition().y
        );
        valueText.setOutlineThickness(2.f);

        updateHandlePosition();
    }

    void updateHandlePosition()
    {
        float percent = (value - min) / (max - min);
        float trackX = track.getPosition().x;
        float trackY = track.getPosition().y;
        float width = track.getSize().x;

        handle.setPosition(trackX + percent * width, trackY + track.getSize().y / 2.f);

        fill.setSize({ percent * width, 6.f });

        valueText.setString(to_string(static_cast<int>(value)) + "%");
        valueText.setOrigin(valueText.getLocalBounds().width / 2.f, valueText.getLocalBounds().height / 2.f);
        FloatRect labelGlobal = label.getGlobalBounds();
        valueText.setPosition(
            labelGlobal.left + labelGlobal.width + 70.f,
            label.getPosition().y
        );

    }
};

inline std::ostream& operator<<(std::ostream& os, const SliderComponent& s) {
    os << "SliderComponent\n";
    os << "\"" << s.type << "\" "
        << s.min << " " << s.max << " " << s.value << " "
        << s.isDragging << " "
        << s.track.getPosition().x << " " << s.track.getPosition().y << " "
        << s.track.getSize().x << '\n';
    return os;
}

inline std::istream& operator>>(std::istream& is, SliderComponent& s) {
    std::string labelText;
    char quote;
    is >> std::ws >> quote;
    std::getline(is, labelText, '"');

    float min, max, value;
    bool isDragging;
    float x, y, width;

    is >> min >> max >> value >> isDragging >> x >> y >> width;

    s.type = labelText;
    s.min = min;
    s.max = max;
    s.value = value;
    s.isDragging = isDragging;

    // Font
    s.font = std::make_shared<sf::Font>();
    if (!s.font->loadFromFile("assets/Font/Minecraft-Regular.otf")) {
        std::cerr << "[SliderComponent] Failed to load font\n";
    }

    // Track
    s.track.setSize({ width, 20.f });
    s.track.setPosition(x, y);
    s.track.setFillColor(sf::Color(60, 60, 60, 200));
    s.track.setOutlineColor(sf::Color::White);
    s.track.setOutlineThickness(1.f);

    // Fill
    s.fill.setSize({ width, 6.f });
    s.fill.setPosition(x, y);
    s.fill.setFillColor(sf::Color(100, 200, 250, 220));

    // Handle
    s.handle.setSize({ 40.f, 40.f });
    s.handle.setOrigin(20.f, 20.f);  // center
    s.handle.setFillColor(sf::Color::White);

    // Label
    s.label.setFont(*s.font);
    s.label.setString(labelText);
    s.label.setCharacterSize(40);
    s.label.setFillColor(sf::Color::White);
    s.label.setOutlineThickness(2.f);
    sf::FloatRect labelBounds = s.label.getLocalBounds();
    s.label.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
    s.label.setPosition(x + width / 2.f - 40.f, y - 50.f);

    // Value text
    s.valueText.setFont(*s.font);
    s.valueText.setCharacterSize(40);
    s.valueText.setFillColor(sf::Color::White);
    s.valueText.setOutlineThickness(2.f);

    s.updateHandlePosition();  

    return is;
}


