#pragma once
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../Managers/EntityManager.h"
#include "../Managers/World.h"

using namespace std;
using namespace sf;

struct TextComponent
{
	Text txt;
	std::shared_ptr<sf::Font> font;
	FloatRect bound;
	Color originColor;
	bool isHover;


	TextComponent() = default;
	function<void(EntityID, World&)> onClick;
	TextComponent(const string& str, int n, const string& fontPath, Color color, const Vector2f& position, bool hover, sf::Color outline, float thickness);
	bool contains(Vector2f point) const;
	bool tryClick(Vector2f mousePos, EntityID entityId, World& world);
	void tryHover(Vector2f mousePos, EntityID entityId, World& world);
	void setString(string newTxt);
};