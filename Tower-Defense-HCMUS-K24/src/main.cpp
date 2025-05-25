#include "Gameplay.h"
#include "SFML/Graphics.hpp"
using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1200), "Tower Defense");
	Gameplay game;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.display();
	}
	
	return 0;
}
