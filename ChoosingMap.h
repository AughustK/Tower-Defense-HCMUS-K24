#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "BackButtom.h"
class ChoosingMap 
{
private:
    sf::Font font;
    sf::Text title;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    std::vector<std::unique_ptr<sf::Texture>> mapTextures; 
    std::vector<sf::Sprite> mapSprites;
    int selectedMapIndex;
    std::vector<float> originalScales;
    std::vector<sf::RectangleShape> hoverRects;
public:
    ChoosingMap();

    void loadAssets();
    void handleEvent(sf::RenderWindow& window, sf::Event& event, bool& mapChosen, int& chosenMap);
    void render(sf::RenderWindow& window);
};
