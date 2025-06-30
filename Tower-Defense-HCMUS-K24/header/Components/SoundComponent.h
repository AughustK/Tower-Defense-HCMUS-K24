#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <iostream>

using std::string;

struct SoundComponent {
    std::shared_ptr<sf::SoundBuffer> buffer;
    std::shared_ptr<sf::Sound> sound;

    bool playOnce = true;
    bool triggered = false;

    SoundComponent() = default;

    SoundComponent(const string& soundPath, bool loop) : playOnce(!loop)
    {
        buffer = std::make_shared<sf::SoundBuffer>();
        sound = std::make_shared<sf::Sound>(); 

        if (!buffer->loadFromFile(soundPath)) {
            std::cerr << "Failed to load sound: " << soundPath << '\n';
        }
        else {
            sound->setBuffer(*buffer);
            sound->setLoop(!playOnce);
        }
    }
};
