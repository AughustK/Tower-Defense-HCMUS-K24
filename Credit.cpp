#include "Credit.h"
#include <SFML/Graphics.hpp>

Credit::Credit() {}

bool Credit::loadAssets()
{
    if (!backgroundCreditsTexture.loadFromFile("assets/CreditBackground2Fixed.png")) return false;
    backgroundCreditsSprite.setTexture(backgroundCreditsTexture);
    backgroundCreditsSprite.setPosition(0, 0);


    if (!creditTexture.loadFromFile("assets/testCreditV2.png")) return false;
    creditSprite.setTexture(creditTexture);
    creditSprite.setScale(1.2, 1.2);
    float centerX = (WindowWidth - creditSprite.getGlobalBounds().width) / 2.f;
    creditSprite.setPosition(centerX, 0.f);

    if (!exitButtonTexture.loadFromFile("A_Back2.png")) return false;
    exitButtonSprite.setTexture(exitButtonTexture);
    exitButtonSprite.setPosition(5.f, 5.f);
    exitButtonSprite.setScale(5, 5);

    // Setup view
    view = sf::View(sf::FloatRect(0, 0, WindowWidth, WindowHeight));
    maxScroll = creditSprite.getGlobalBounds().top + creditSprite.getGlobalBounds().height - WindowHeight;
    return true;
}

bool Credit::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffsetY -= event.mouseWheelScroll.delta * 90.f;

        if (scrollOffsetY < 0) scrollOffsetY = 0;
        if (scrollOffsetY > maxScroll) scrollOffsetY = maxScroll;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (exitButtonSprite.getGlobalBounds().contains(mousePos)) {
            return true;
        }
    }

    return false;
}

void Credit::draw(sf::RenderWindow& window)
{
    window.draw(backgroundCreditsSprite);
    view.setCenter(WindowWidth / 2.f, scrollOffsetY + WindowHeight / 2.f);
    window.setView(view);
    window.draw(creditSprite);
    window.setView(window.getDefaultView());
    window.draw(exitButtonSprite);
}