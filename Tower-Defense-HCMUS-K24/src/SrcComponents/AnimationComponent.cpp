#include "../../header/Components/AnimationComponent.h"

void AnimationComponent::addAnimation(const std::string& name, const Animation& animation) 
{
    animations[name] = animation;
    if (currentAnimation.empty()) 
    {
        currentAnimation = name;
    }
}

void AnimationComponent::play(const std::string& name) 
{
    if (animations.find(name) != animations.end()) {
        if (currentAnimation != name) 
        {
            currentAnimation = name;
            currentFrame = 0;
            elapsedTime = 0.0f;
            animations[currentAnimation].finished = false;
        }
        isPlaying = true;
    }
}

void AnimationComponent::stop() 
{
    isPlaying = false;
    currentFrame = 0;
    elapsedTime = 0.0f;
    if (!currentAnimation.empty())
        animations[currentAnimation].finished = false;
}

void AnimationComponent::setLoop(bool loop) 
{
    if (!currentAnimation.empty())
        animations[currentAnimation].loop = loop;
}

bool AnimationComponent::isFinished() const 
{
    if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
        return true;
    return animations.at(currentAnimation).finished;
}

sf::IntRect AnimationComponent::getCurrentRect() const {
    if (currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
        return sf::IntRect(0, 0, 0, 0);
    const Animation& anim = animations.at(currentAnimation);
    if (anim.frames.empty())
        return sf::IntRect(0, 0, 0, 0);
    return anim.frames[currentFrame].rect;
}

void AnimationComponent::updateAnimation(float deltaTime, World& world)
{
    for (auto entity : entities)
    {
        if (world.hasComponent<SpriteComponent>(entity) && world.hasComponent<AnimationComponent>(entity))
        {
            auto& sprite = world.getComponent<SpriteComponent>(entity);
            auto& animation = world.getComponent<AnimationComponent>(entity);
            
            // Update position from PositionComponent
            if (world.hasComponent<PositionComponent>(entity))
            {
                auto& pos = world.getComponent<PositionComponent>(entity);
                sprite.sprite.setPosition(pos.x, pos.y);
            }
            
            // Update animation using the AnimationComponent
            animation.update(deltaTime);
            
            // Apply the current frame to the sprite
            sprite.sprite.setTextureRect(animation.getCurrentRect());
            
            sf::FloatRect bounds = sprite.sprite.getLocalBounds();
            sprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        }
    }
}

// void AnimationComponent::update(float deltaTime) 
    // {
    //     if (!isPlaying || currentAnimation.empty() || animations.find(currentAnimation) == animations.end())
    //         return;
    //     Animation& anim = animations[currentAnimation];
    //     if (anim.frames.empty() || anim.finished)
    //         return;
    //     elapsedTime += deltaTime;
    //     while (elapsedTime >= anim.frames[currentFrame].duration) {
    //         elapsedTime -= anim.frames[currentFrame].duration;
    //         currentFrame++;
    //         if (currentFrame >= anim.frames.size()) {
    //             if (anim.loop) {
    //                 currentFrame = 0;
    //             }
    //             else {
    //                 currentFrame = anim.frames.size() - 1;
    //                 anim.finished = true;
    //                 isPlaying = false;
    //                 break;
    //             }
    //         }
    //     }
    // }