#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

struct SpriteComponent {
    sf::Sprite sprite;
    std::function<void(int)> onClick;

    SpriteComponent() = default;

    SpriteComponent(const sf::Texture& texture,
        const sf::Vector2f& position = { 0.f, 0.f },
        const sf::Vector2f& scale = { 1.f, 1.f })
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(scale);
    }

    void setTexture(const sf::Texture& texture) {
        sprite.setTexture(texture);
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setScale(float x, float y) {
        sprite.setScale(x, y);
    }

    bool contains(const sf::Vector2f& point) const {
        return sprite.getGlobalBounds().contains(point);
    }

    void tryClick(const sf::Vector2f& mousePos, int entityId) {
        if (contains(mousePos) && onClick) {
            onClick(entityId);
        }
    }
};
