#pragma once
#include <iostream>
#include "System.h"
#include "../Managers/World.h"
#include "../Components/SoundComponent.h"

using std::cout;

class SoundSystem : public System
{

public:
    float globalVolume = 100.f;
    SoundSystem() = default;

    void update(float deltaTime) override {}

    void play(World& world) {
        for (EntityID entity : entities) {
            auto& soundComp = world.getComponent<SoundComponent>(entity);

            if (soundComp.sound)
            {
                soundComp.sound->setVolume(globalVolume);
            }

            if (soundComp.triggered)
            {
                if (soundComp.sound->getStatus() != sf::Sound::Playing)
                {
                    soundComp.sound->play();
                }
                soundComp.triggered = false;
            }
        }
    }
};
