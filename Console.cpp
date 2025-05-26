#include "Console.h"
#include "Menu.h"
#include "Credit.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

Console::Console()
    : window(VideoMode(1920, 1080), "Tower Defense") {
}

void Console::run()
{
    Menu menu(1920, 1080);
    Credit* creditScreen = nullptr;
    bool isShowingCredit = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // is in Credit
            if (isShowingCredit && creditScreen) {
                // handleEvent == true => wanna return to menu
                if (creditScreen->handleEvent(event, window)) {
                    delete creditScreen;
                    creditScreen = nullptr;
                    isShowingCredit = false;
                }
                // Skip other tasks
                continue;
            }

            // is in menu
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i pixelPos = sf::Mouse::getPosition(window);
                Vector2f localPosition = window.mapPixelToCoords(pixelPos);

                // Click exit
                if (menu.mainMenu0[2].getGlobalBounds().contains(localPosition))
                {
                    window.close();
                }

                // Click Credit
                if (menu.mainMenu1[1].getGlobalBounds().contains(localPosition)) {
                    if (!creditScreen) {
                        creditScreen = new Credit();
                        if (!creditScreen->loadAssets()) return;
                    }
                    isShowingCredit = true;
                }
            }
        }

        window.clear();
        //draw Credit
        if (isShowingCredit && creditScreen)
        {
            creditScreen->draw(window);
        }
        else
        {
            //Hover
            menu.updateHover(window);
            //draw Window
            menu.draw(window);
        }
        window.display();
    }

    delete creditScreen;
}
