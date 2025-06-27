
#include "Console.h"
#include "Menu.h"
#include "Credit.h"
#include "ChoosingMap.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

Console::Console()
    : window(VideoMode(1920, 1080), "Tower Defense") {
}

void Console::run()
{
    Menu menu(1920, 1080);
    Credit* creditScreen = nullptr;
    ChoosingMap choosingMap;
    bool isMapChosen = false;
    int selectedMap = -1;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // CREDIT SCREEN
            if (currentState == AppState::Credit && creditScreen) {
                if (creditScreen->handleEvent(event, window)) {
                    delete creditScreen;
                    creditScreen = nullptr;
                    currentState = AppState::Menu;
                }
                continue;
            }

            // CHOOSING MAP SCREEN
            if (currentState == AppState::ChoosingMap) {
                choosingMap.handleEvent(window, event, isMapChosen, selectedMap);
                if (isMapChosen) {
                    cout << "Map " << selectedMap + 1 << " selected!" << endl;
                    // future: start game with selectedMap
                }
                continue;
            }

            // MENU SCREEN
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i pixelPos = Mouse::getPosition(window);
                Vector2f localPosition = window.mapPixelToCoords(pixelPos);

                // Click Play
                if (menu.mainMenu0[0].getGlobalBounds().contains(localPosition)) {
                    currentState = AppState::ChoosingMap;
                }

                // Click Exit
                if (menu.mainMenu0[2].getGlobalBounds().contains(localPosition)) {
                    window.close();
                }

                // Click Credit
                if (menu.mainMenu1[1].getGlobalBounds().contains(localPosition)) {
                    if (!creditScreen) {
                        creditScreen = new Credit();
                        if (!creditScreen->loadAssets()) {
                            delete creditScreen;
                            creditScreen = nullptr;
                            continue;
                        }
                    }
                    currentState = AppState::Credit;
                }
            }
        }

        window.clear();

        if (currentState == AppState::Credit && creditScreen) {
            creditScreen->draw(window);
        }
        else if (currentState == AppState::ChoosingMap) {
            choosingMap.render(window);
        }
        else {
            menu.updateHover(window);
            menu.draw(window);
        }

        window.display();
    }

    delete creditScreen;
}
