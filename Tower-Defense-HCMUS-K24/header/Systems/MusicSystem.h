#pragma once
#include <iostream>
#include "../Systems/System.h"
#include "../Managers/World.h"
#include "../Components/MusicComponent.h"

using std::cout;

class MusicSystem : public System 
{

public:
    float globalVolume = 100.f;
    MusicSystem() = default;

    void update(float deltaTime) override {}

    void play(World& world) 
    {
        for (EntityID entity : entities) 
        {
            auto& musicComp = world.getComponent<MusicComponent>(entity);

            if (musicComp.music) 
            {
                musicComp.music->setVolume(globalVolume);
            }

            if (musicComp.triggered && musicComp.music) 
            {
                if (musicComp.music->getStatus() != sf::Music::Playing) {
                    musicComp.music->play();
                }
            }
        }
    }

    void pauseAll(World& world)
    {
        for (EntityID entity : entities)
        {
            auto& musicComp = world.getComponent<MusicComponent>(entity);
            if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
                musicComp.music->pause();
            }
        }
    }

    void resumeAll(World& world)
    {
        for (EntityID entity : entities)
        {
            auto& musicComp = world.getComponent<MusicComponent>(entity);
            if (musicComp.music && musicComp.triggered) {
                musicComp.music->play();
            }
        }
    }

};

