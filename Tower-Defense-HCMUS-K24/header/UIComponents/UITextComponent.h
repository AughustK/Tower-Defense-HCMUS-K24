#pragma once
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"

using namespace std;
using namespace sf;

struct TextComponent {
	Text txt;
	std::shared_ptr<sf::Font> font;
	FloatRect bound;

	TextComponent() = default;
	function<void(EntityID, World&)> onClick;

	TextComponent(const string& str,
		int n, const string& fontPath,
		Color color, const Vector2f& position )
	{
		txt.setString(str);
		txt.setCharacterSize(n);
		font = make_shared<Font>();
		if (!font->loadFromFile(fontPath)) {
			std::cerr << "Failed to load font: " << fontPath << '\n';
		}
		txt.setFont(*font);
		txt.setFillColor(color);
		txt.setPosition(position);
		bound = txt.getGlobalBounds();
		txt.setOrigin(bound.width / 2, bound.height / 2);
		txt.setPosition(position);
	}

	bool contains(Vector2f point) const {
		return txt.getGlobalBounds().contains(point);
	}

	void tryClick(Vector2f mousePos, EntityID entityId, World& world) {
		if (contains(mousePos) && onClick) {
			onClick(entityId, world);
		}
	}
};