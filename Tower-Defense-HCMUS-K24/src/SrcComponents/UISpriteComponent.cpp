#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Managers/EntityManager.h"
#include "../../header/Managers/World.h"

SpriteComponent::SpriteComponent(const std::string& path, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover)
    : isHover(isHover)
{
    texture = make_shared<Texture>();
    texture->loadFromFile(path);
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
}

SpriteComponent::SpriteComponent(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover)
    : isHover(isHover)
{
    this->texture = std::make_shared<sf::Texture>(texture);
    sprite.setTexture(*this->texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
}

bool SpriteComponent::contains(const sf::Vector2f& point) const
{
    return sprite.getGlobalBounds().contains(point);
}

bool SpriteComponent::tryClick(Vector2f mousePos, EntityID entityId, World& world)
{
    if (contains(mousePos) && onClick)
    {
        onClick(entityId, world);
        return true;
    }
    return false;
}

void SpriteComponent::setTxt(string path)
{
    texture = make_shared<Texture>();
    texture->loadFromFile(path);
    sprite.setTexture(*texture);
}

