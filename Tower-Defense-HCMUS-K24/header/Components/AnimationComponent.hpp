#pragma once
#include <SFML/Graphics.hpp>

class AnimationComponent {
private:
    const sf::Texture* texture;
    int frameWidth;
    int frameHeight;
    int numFrames;
    int currentFrame;
    float timePerFrame;
    float timeAccumulator;
    bool isPlaying;
public:
    AnimationComponent();

    void setSpriteSheet(const sf::Texture& texture, int frameWidth, int frameHeight, int numFrames);
    void setFrameTime(float timePerFrame);
    void play();
    void stop();
    void update(float deltaTime);
    void applyToSprite(sf::Sprite& sprite);
    
};