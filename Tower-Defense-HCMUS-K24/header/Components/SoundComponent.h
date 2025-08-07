#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <iostream>

using std::string;

struct SoundComponent
{
    std::shared_ptr<sf::SoundBuffer> buffer;
    std::shared_ptr<sf::Sound> sound;

    bool playOnce = true;
    bool triggered = false;
    std::string path;

    SoundComponent() = default;

    SoundComponent(const string& soundPath, bool loop) : playOnce(!loop), path(soundPath)
    {
        buffer = std::make_shared<sf::SoundBuffer>();
        sound = std::make_shared<sf::Sound>();

        if (!buffer->loadFromFile(soundPath))
        {
            std::cerr << "Failed to load sound: " << soundPath << '\n';
        }
        else {
            sound->setBuffer(*buffer);
            sound->setLoop(!playOnce);
        }
    }

    SoundComponent(const string& soundPath, bool loop, float volume)
        : playOnce(!loop), path(soundPath)
    {
        buffer = std::make_shared<sf::SoundBuffer>();
        sound = std::make_shared<sf::Sound>();

        if (!buffer->loadFromFile(soundPath)) {
            std::cerr << "Failed to load sound: " << soundPath << '\n';
        }
        else {
            sound->setBuffer(*buffer);
            sound->setLoop(!playOnce);
            sound->setVolume(volume);
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, const SoundComponent& s) {
    os << "SoundComponent\n";
    os << s.path << '\n'
        << s.playOnce << '\n'
        << s.triggered << '\n';
    return os;
}

inline std::istream& operator>>(std::istream& is, SoundComponent& s) {
    std::getline(is, s.path);
    is >> s.playOnce >> s.triggered;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    s.buffer = std::make_shared<sf::SoundBuffer>();
    s.sound = std::make_shared<sf::Sound>();

    if (!s.buffer->loadFromFile(s.path)) {
        std::cerr << "[SoundComponent] Failed to load sound: " << s.path << '\n';
    }
    else {
        s.sound->setBuffer(*s.buffer);
        s.sound->setLoop(!s.playOnce);
    }

    return is;
}