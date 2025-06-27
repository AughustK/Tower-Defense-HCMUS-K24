#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

// Represents one frame of animation
struct AnimationFrame {
    sf::IntRect rect; // Rectangle within texture
    float duration;   // Duration of this specific frame
    AnimationFrame(const sf::IntRect& r, float d) : rect(r), duration(d) {}
};

// Represents a complete animation sequence (like "walk", "attack", etc.)
struct Animation {
    std::vector<AnimationFrame> frames;
    bool loop;
    bool finished;
    Animation() : loop(true), finished(false) {}
};

// The actual ECS component
struct AnimationComponent {
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation;
    size_t currentFrame;
    float elapsedTime;
    bool isPlaying;

    AnimationComponent() : currentFrame(0), elapsedTime(0.0f), isPlaying(false) {}

    // Add a new animation
    void addAnimation(const std::string& name, const Animation& animation) {
        animations[name] = animation;
        if (currentAnimation.empty()) {
            currentAnimation = name;
        }
    }

    // Play an animation (reset if switching)
    void play(const std::string& name) {
        if (animations.find(name) != animations.end()) {
            if (currentAnimation != name) {
                currentAnimation = name;
                currentFrame = 0;
                elapsedTime = 0.0f;
                animations[currentAnimation].finished = false;
            }
            isPlaying = true;
        }
    }

    // Pause the animation
    void pause() { isPlaying = false; }

    // Resume the animation
    void resume() { if (!isFinished()) isPlaying = true; }

    // Stop and reset the animation
    void stop() {
        isPlaying = false;
        currentFrame = 0;
        elapsedTime = 0.0f;
        if (!currentAnimation.empty())
            animations[currentAnimation].finished = false;
    }

    // Set whether the current animation should loop
    void setLoop(bool loop) {
        if (!currentAnimation.empty())
            animations[currentAnimation].loop = loop;
    }

    // Returns true if the current animation is finished (for non-looping)
    bool isFinished() const {
        if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
            return true;
        return animations.at(currentAnimation).finished;
    }

    // Get the current frame index
    size_t getCurrentFrameIndex() const { return currentFrame; }

    // Get the current frame rectangle
    sf::IntRect getCurrentRect() const {
        if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
            return sf::IntRect(0, 0, 0, 0);
        const Animation& anim = animations.at(currentAnimation);
        if (anim.frames.empty())
            return sf::IntRect(0, 0, 0, 0);
        return anim.frames[currentFrame].rect;
    }

    // Update the animation based on delta time
    void update(float deltaTime) {
        if (!isPlaying || currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
            return;
        Animation& anim = animations[currentAnimation];
        if (anim.frames.empty() || anim.finished)
            return;
        elapsedTime += deltaTime;
        while (elapsedTime >= anim.frames[currentFrame].duration) {
            elapsedTime -= anim.frames[currentFrame].duration;
            currentFrame++;
            if (currentFrame >= anim.frames.size()) {
                if (anim.loop) {
                    currentFrame = 0;
                } else {
                    currentFrame = anim.frames.size() - 1;
                    anim.finished = true;
                    isPlaying = false;
                    break;
                }
            }
        }
    }
};