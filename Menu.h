#pragma once
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Menu {
	int selected = -1;
	Font font, titleFont;
	Sprite bg;
	Texture setting, credit, help, backgroundTexture;
public:
	Text mainMenu0[4];
	Sprite mainMenu1[3];
	Menu(float width, float height);
	void draw(RenderWindow& window);
	void setSelected(int n);
	int pressed() {
		return selected;
	}
	void updateHover(const sf::RenderWindow& window);
	~Menu();
};
