#pragma once
#include "../../header/Managers/World.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

// Represents one frame of animation
struct AnimationFrame 
{
    sf::IntRect rect; // Rectangle within texture
    float duration;   // Duration of this specific frame
    AnimationFrame(const sf::IntRect& r, float d) : rect(r), duration(d) {}
};

// Represents a complete animation sequence (like "walk", "attack", etc.)
struct Animation 
{
    std::vector<AnimationFrame> frames;
    bool loop;
    bool finished;
    Animation() : loop(true), finished(false) {}
};

// The actual ECS component
struct AnimationComponent 
{
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation;
    size_t currentFrame;
    float elapsedTime;
    bool isPlaying;

    AnimationComponent() : currentFrame(0), elapsedTime(0.0f), isPlaying(false) {}

    // Add a new animation
    void addAnimation(const std::string& name, const Animation& animation);

    // Play an animation (reset if switching)
    void play(const std::string& name);

    // Pause the animation
    void pause() { isPlaying = false; }

    // Resume the animation
    void resume() { if (!isFinished()) isPlaying = true; }

    // Stop and reset the animation
    void stop();

    // Set whether the current animation should loop
    void setLoop(bool loop);

    // Returns true if the current animation is finished (for non-looping)
    bool isFinished() const 
    {
        if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
            return true;
        return animations.at(currentAnimation).finished;
    }

    // Get the current frame index
    size_t getCurrentFrameIndex() const { return currentFrame; }

    // Get the current frame rectangle
    sf::IntRect getCurrentRect() const;

    // Update the animation based on delta time
    void updateAnimation(float deltaTime, World& world);
};