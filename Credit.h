#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

const int WindowWidth = 1920, WindowHeight = 1080;

class Credit {
private:
    sf::Texture creditTexture;
    sf::Sprite creditSprite;

    sf::Texture exitButtonTexture;
    sf::Sprite exitButtonSprite;

    sf::Texture backgroundCreditsTexture;
    sf::Sprite backgroundCreditsSprite;

    sf::View view;
    float scrollOffsetY = 0.f;
    float maxScroll = 0.f;

public:
    Credit();
    bool loadAssets();
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
};
