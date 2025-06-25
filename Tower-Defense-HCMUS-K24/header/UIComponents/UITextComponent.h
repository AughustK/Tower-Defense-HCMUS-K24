#pragma once
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct TextComponent {
	Text txt;
	Font font;

	TextComponent() = default;
	function<void(int)> onClick;

	void setString(const string &str) {
		txt.setString(str);
	}
	
	void setSize(int &n) {
		txt.setCharacterSize(n);
	}

	void setFont(const string &fontPath) {
		font.loadFromFile(fontPath);
		txt.setFont(font);
	}

	void setColor(Color color) {
		txt.setFillColor(color);
	}

	bool contains(Vector2f point) const {
		return txt.getGlobalBounds().contains(point);
	}

	void tryClick(Vector2f mousePos, int entityId) {
		if (contains(mousePos) && onClick) {
			onClick(entityId);
		}
	}
};