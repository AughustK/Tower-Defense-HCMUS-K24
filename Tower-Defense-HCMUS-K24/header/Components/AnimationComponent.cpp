#include "AnimationComponent.hpp"

AnimationComponent::AnimationComponent()
    : texture(nullptr), frameWidth(0), frameHeight(0), numFrames(0),
      currentFrame(0), timePerFrame(0.1f), timeAccumulator(0.f), isPlaying(false) {}

void AnimationComponent::setSpriteSheet(const sf::Texture& tex, int w, int h, int frames) {
    texture = &tex;
    frameWidth = w;
    frameHeight = h;
    numFrames = frames;
    currentFrame = 0;
}

void AnimationComponent::setFrameTime(float time) {
    timePerFrame = time;
}

void AnimationComponent::play() {
    isPlaying = true;
    timeAccumulator = 0.f;
    currentFrame = 0;
}

void AnimationComponent::stop() {
    isPlaying = false;
    currentFrame = 0;
}

void AnimationComponent::update(float deltaTime) {
    if (!isPlaying || !texture) return;

    timeAccumulator += deltaTime;
    while (timeAccumulator >= timePerFrame) {
        timeAccumulator -= timePerFrame;
        currentFrame = (currentFrame + 1) % numFrames;
    }
}

void AnimationComponent::applyToSprite(sf::Sprite& sprite) {
    if (!texture) return;
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(
        frameWidth * currentFrame, 0,
        frameWidth, frameHeight
    ));
}
