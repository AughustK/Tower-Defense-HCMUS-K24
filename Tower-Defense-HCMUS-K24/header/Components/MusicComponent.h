#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <iostream>

using std::string;

struct MusicComponent {
    std::shared_ptr<sf::Music> music;
    bool playOnce = false;
    bool triggered = true;

    MusicComponent() = default;

    MusicComponent(const string& soundPath)
    {
        music = std::make_shared<sf::Music>();

        if (!music->openFromFile(soundPath)) 
        {
            std::cerr << "Failed to open music: " << soundPath << '\n';
        }
        else {
            music->setLoop(!playOnce);
        }
    }
};
