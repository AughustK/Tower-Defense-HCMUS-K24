
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
    std::string spritePath;

    bool isHover;

    SpriteComponent() = default;
    SpriteComponent(const std::string& path, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover = false);
    SpriteComponent(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& scale, bool isHover = false);
    bool contains(const sf::Vector2f& point) const;
    bool tryClick(Vector2f mousePos, EntityID entityId, World& world);
    void setTxt(string path);
};

inline std::ostream& operator<<(std::ostream& os, const SpriteComponent& s) {
    os << "SpriteComponent\n";
    os << "\"" << s.spritePath << "\" "
        << s.sprite.getPosition().x << " " << s.sprite.getPosition().y << " "
        << s.sprite.getScale().x << " " << s.sprite.getScale().y << " "
        << static_cast<int>(s.isHover) << " " << s.frameCount << " " << s.frameRate << "\n";
    return os;
}

inline std::istream& operator>>(std::istream& is, SpriteComponent& s) {
    char quote;
    std::string path;
    float posX, posY, scaleX, scaleY;
    int isHoverInt;

    is >> std::ws >> quote;
    std::getline(is, path, '"');

    is >> posX >> posY >> scaleX >> scaleY >> isHoverInt >> s.frameCount >> s.frameRate;

    s.spritePath = path;
    s.texture = std::make_shared<sf::Texture>();
    if (!s.texture->loadFromFile(path)) {
        std::cerr << "[SpriteComponent] Failed to load texture: " << path << "\n";
    }

    s.sprite.setTexture(*s.texture);
    s.sprite.setPosition({ posX, posY });
    s.sprite.setScale({ scaleX, scaleY });
    s.isHover = (isHoverInt != 0);

    if (s.frameCount > 1) {
        auto texSize = s.texture->getSize();
        int frameWidth = texSize.x / std::max(1, s.frameCount);
        s.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, texSize.y));

        sf::FloatRect bounds = s.sprite.getLocalBounds();
        s.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    return is;
}