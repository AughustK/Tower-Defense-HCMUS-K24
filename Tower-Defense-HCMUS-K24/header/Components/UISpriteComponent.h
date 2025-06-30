#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"

using namespace sf;
using namespace std;

struct SpriteComponent 
{
    sf::Sprite sprite;
    std::shared_ptr<sf::Texture> texture;
    std::function<void(EntityID, World&)> onClick;

    SpriteComponent() = default;
    SpriteComponent(const std::string& path, const sf::Vector2f& position, const sf::Vector2f& scale);
    bool contains(const sf::Vector2f& point) const;
    void tryClick(Vector2f mousePos, EntityID entityId, World& world);
};
