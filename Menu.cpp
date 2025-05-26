#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

Menu::Menu(float width, float height) {
	font.loadFromFile("D:/HCMUS/OOP/Project/Font/LowresPixel-Regular.otf");
	titleFont.loadFromFile("D:/HCMUS/OOP/Project/Font/Karma Future.otf");

	mainMenu0[0].setFont(font);
	mainMenu0[0].setFillColor(Color::White);
	mainMenu0[0].setString("Play");
	mainMenu0[0].setCharacterSize(90);
	FloatRect bounds1 = mainMenu0[0].getGlobalBounds();
	mainMenu0[0].setOrigin(bounds1.width / 2, bounds1.height / 2);
	mainMenu0[0].setPosition(Vector2f(1920 / 2, 460 + 50));

	mainMenu0[1].setFont(font);
	mainMenu0[1].setFillColor(Color::White);
	mainMenu0[1].setString("Continue");
	mainMenu0[1].setCharacterSize(90);
	FloatRect bounds2 = mainMenu0[1].getGlobalBounds();
	mainMenu0[1].setOrigin(bounds2.width / 2, bounds2.height / 2);
	mainMenu0[1].setPosition(Vector2f(1920 / 2, 360 + 50 + 30 + 200));

	mainMenu0[2].setFont(font);
	mainMenu0[2].setFillColor(Color::White);
	mainMenu0[2].setString("Exit");
	mainMenu0[2].setCharacterSize(90);
	FloatRect bounds3 = mainMenu0[2].getGlobalBounds();
	mainMenu0[2].setOrigin(bounds3.width / 2, bounds3.height / 2);
	mainMenu0[2].setPosition(Vector2f(1920 / 2, 360 + 3 * 100 + 2 * 30 + 50));

	mainMenu0[3].setFont(titleFont);
	mainMenu0[3].setFillColor(Color::Cyan);
	mainMenu0[3].setString("TOWER DEFENSE");
	mainMenu0[3].setCharacterSize(140);
	FloatRect bounds4 = mainMenu0[3].getGlobalBounds();
	mainMenu0[3].setOrigin(bounds4.width / 2, bounds4.height / 2);
	mainMenu0[3].setPosition(Vector2f(1920 / 2, 360));

	//size=100
	setting.loadFromFile("D:/HCMUS/OOP/Project/Picture/Icon/Settings/A_Settings2.png");
	mainMenu1[0].setTexture(setting);
	mainMenu1[0].setScale(Vector2f(5, 5));
	mainMenu1[0].setPosition(Vector2f(1620, 980));

	credit.loadFromFile("D:/HCMUS/OOP/Project/Picture/Icon/Level/A_Level2.png");
	mainMenu1[1].setTexture(credit);
	mainMenu1[1].setScale(Vector2f(5, 5));
	mainMenu1[1].setPosition(Vector2f(1720, 980));

	help.loadFromFile("D:/HCMUS/OOP/Project/Picture/Icon/Unlock/A_Unlock2.png");
	mainMenu1[2].setTexture(help);
	mainMenu1[2].setScale(Vector2f(5, 5));
	mainMenu1[2].setPosition(Vector2f(1820, 980));

	backgroundTexture.loadFromFile("D:/HCMUS/OOP/Project/Picture/MenuV2-1.png");
	bg.setTexture(backgroundTexture);
}

void Menu::updateHover(const sf::RenderWindow& window)
{
	Vector2i pixelPos = sf::Mouse::getPosition(window);
	Vector2f localPos = window.mapPixelToCoords(pixelPos);
	for (int i = 0; i < 3; ++i) 
	{
		if (mainMenu0[i].getGlobalBounds().contains(localPos)) {
			mainMenu0[i].setFillColor(sf::Color::Yellow); 
		}
		else {
			mainMenu0[i].setFillColor(sf::Color::White);   
		}
	}
}

Menu::~Menu() {

}

void Menu::setSelected(int n) {
	selected = n;
}

void Menu::draw(RenderWindow& window) {
	window.draw(bg);
	for (int i = 0; i < 4; i++) {
		window.draw(mainMenu0[i]);
	}
	for (int i = 0; i < 3; i++) {
		window.draw(mainMenu1[i]);
	}
}
