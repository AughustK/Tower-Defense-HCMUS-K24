#include "../header/Managers/World.h"
#include "../header/GameStates/MainMenu.h"
#include "../header/Managers/EntityManager.h"

int main()
{
    World world;

    world.init(); //
    world.setState(std::make_unique<MainMenu>());  //

    sf::Clock clock;

    while (world.isRunning())
    {
        sf::Event event;
        while (world.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                world.requestClose();

            world.handleEvent(event);
        }

        float deltaTime = clock.restart().asSeconds();
        world.update(deltaTime);

        world.window.clear(sf::Color::Black);
        world.render();
        world.window.display();
    }
    return 0;
}

