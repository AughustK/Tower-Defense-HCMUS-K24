#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Managers/EntityManager.h"
#include "../../header/Managers/World.h"

SpriteComponent::SpriteComponent(const std::string& path, const sf::Vector2f& position, const sf::Vector2f& scale)
{
    texture = make_shared<Texture>();
    texture->loadFromFile(path);
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
}

bool SpriteComponent::contains(const sf::Vector2f& point) const
{
    return sprite.getGlobalBounds().contains(point);
}

void SpriteComponent::tryClick(Vector2f mousePos, EntityID entityId, World& world)
{
    if (contains(mousePos) && onClick) 
    {
        onClick(entityId, world);
    }
}

