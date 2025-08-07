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
	string path;  

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

inline std::ostream& operator<<(std::ostream& os, const MusicComponent& m) 
{
    os << "MusicComponent\n";
    os << m.path << '\n'
        << m.playOnce << '\n'
        << m.triggered << '\n';
    return os;
}

inline std::istream& operator>>(std::istream& is, MusicComponent& m) 
{
    std::getline(is, m.path);

    is >> m.playOnce >> m.triggered;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m.music = std::make_shared<sf::Music>();
    if (!m.music->openFromFile(m.path)) {
        std::cerr << "[MusicComponent] Failed to open music: " << m.path << '\n';
    }
    else {
        m.music->setLoop(!m.playOnce);
    }

    return is;
}