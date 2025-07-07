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

        // Sau khi setString mới lấy đúng localBounds
        FloatRect labelBounds = label.getLocalBounds();
        label.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
        label.setPosition(x + track.getSize().x / 2.f - 40.f, y - 50.f); // lệch trái chút nếu cần chỗ cho %

        // Value text
        valueText.setFont(*font);
        valueText.setCharacterSize(40);
        valueText.setFillColor(sf::Color::White);
        valueText.setString(to_string(static_cast<int>(value)) + "%");

        FloatRect valueBounds = valueText.getLocalBounds();
        valueText.setOrigin(valueBounds.width / 2.f, valueBounds.height / 2.f);
        FloatRect labelGlobal = label.getGlobalBounds();
        valueText.setPosition(
            labelGlobal.left + labelGlobal.width + 70.f,  // 20.f là khoảng cách tùy chỉnh
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