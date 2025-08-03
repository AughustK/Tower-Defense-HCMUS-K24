
#pragma once
#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"
#include <functional>
#include <string>

using namespace sf;
using namespace std;

struct SpriteComponent
{
    int frameCount = 1;
    float frameRate = 1.0f;

    sf::Sprite sprite;
    std::shared_ptr<sf::Texture> texture;
    std::function<void(EntityID, World&)> onClick;

    bool isHover;

    SpriteComponent() = default;
    SpriteComponent(const std::string& path, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover = false);
    SpriteComponent(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover = false);
    bool contains(const sf::Vector2f& point) const;
    bool tryClick(Vector2f mousePos, EntityID entityId, World& world);
    void setTxt(string path);
};
