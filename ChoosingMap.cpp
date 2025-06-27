#include "ChoosingMap.h"
#include <iostream>
using namespace std;

ChoosingMap::ChoosingMap() : selectedMapIndex(-1) {
    loadAssets();
}

void ChoosingMap::loadAssets() {
    if (!font.loadFromFile("PixelFont.otf")) {
        cerr << "Failed to load font\n";
    }

    if (!backgroundTexture.loadFromFile("assets/choosingmap.png")) {
        cerr << "Failed to load background\n";
    }

    background.setTexture(backgroundTexture);
    background.setScale(
        1920.f / backgroundTexture.getSize().x,
        1080.f / backgroundTexture.getSize().y
    );

    title.setFont(font);
    title.setString("CHOOSE A MAP");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::Red);
    title.setPosition(1920 / 2.f - title.getGlobalBounds().width / 2, 60);

    const float buttonWidth = 680;
    const float buttonHeight = 350;
    const float spacingX = 10;
    const float spacingY = 10;
    const float startX = (1920 - 2 * buttonWidth - spacingX) / 2.f;
    const float startY = 250;

    vector<string> mapPaths = {
        "assets/map1.png", "assets/map2.png",
        "assets/map3.png", "assets/map4.png"
    };

    for (int i = 0; i < 4; ++i) {
        auto tex = std::make_unique<sf::Texture>();
        if (!tex->loadFromFile(mapPaths[i])) {
            std::cerr << "Failed to load " << mapPaths[i] << "\n";
            continue;
        }

        int row = i / 2;
        int col = i % 2;
        float x = startX + col * (buttonWidth + spacingX);
        float y = startY + row * (buttonHeight + spacingY);

        sf::Sprite sprite;
        sprite.setTexture(*tex);
        float scaleX = (buttonWidth - 20) / tex->getSize().x;
        float scaleY = (buttonHeight - 20) / tex->getSize().y;
        float scale = std::min(scaleX, scaleY);
        sprite.setScale(scale, scale);
        sprite.setPosition(
            x + (buttonWidth - tex->getSize().x * scale) / 2,
            y + (buttonHeight - tex->getSize().y * scale) / 2
        );

        mapSprites.push_back(sprite);
        originalScales.push_back(scale);
        mapTextures.push_back(std::move(tex));

        
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sf::RectangleShape border(sf::Vector2f(bounds.width, bounds.height));
        border.setPosition(bounds.left, bounds.top);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineThickness(4.f);
        border.setOutlineColor(sf::Color::Transparent);  
        hoverRects.push_back(border);
    }

    
}

void ChoosingMap::handleEvent(sf::RenderWindow& window, sf::Event& event, bool& mapChosen, int& chosenMap) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    
    for (int i = 0; i < 4; ++i) {
        if (mapSprites[i].getGlobalBounds().contains(mousePos)) {
            hoverRects[i].setOutlineColor(sf::Color::Yellow);
            mapSprites[i].setScale(originalScales[i] * 1.05f, originalScales[i] * 1.05f);

            
            sf::FloatRect newBounds = mapSprites[i].getGlobalBounds();
            hoverRects[i].setSize(sf::Vector2f(newBounds.width, newBounds.height));
            hoverRects[i].setPosition(newBounds.left, newBounds.top);
        }
        else {
            hoverRects[i].setOutlineColor(sf::Color::Transparent);
            mapSprites[i].setScale(originalScales[i], originalScales[i]);

            
            sf::FloatRect normalBounds = mapSprites[i].getGlobalBounds();
            hoverRects[i].setSize(sf::Vector2f(normalBounds.width, normalBounds.height));
            hoverRects[i].setPosition(normalBounds.left, normalBounds.top);
        }
    }

    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < 4; ++i) {
            if (mapSprites[i].getGlobalBounds().contains(mousePos)) {
                selectedMapIndex = i;
                mapChosen = true;
                chosenMap = i;
                return;
            }
        }
    }

}

void ChoosingMap::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(background);
    window.draw(title);

    for (int i = 0; i < 4; ++i) {
        window.draw(hoverRects[i]);
        window.draw(mapSprites[i]);
    }
    window.display();
}
