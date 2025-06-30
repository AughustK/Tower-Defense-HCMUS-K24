#include <iostream>
#include "../../header/Components/UITextComponent.h"

TextComponent::TextComponent(const string& str, int n, const string& fontPath, Color color, const Vector2f& position, bool hover)
	:isHover(hover)
{
	txt.setString(str);
	txt.setCharacterSize(n);
	font = make_shared<Font>();
	if (!font->loadFromFile(fontPath)) 
	{
		std::cerr << "Failed to load font: " << fontPath << '\n';
	}
	txt.setFont(*font);
	originColor = color;
	txt.setFillColor(color);
	txt.setPosition(position);
	bound = txt.getGlobalBounds();
	txt.setOrigin(bound.width / 2, bound.height / 2);
	txt.setPosition(position);
}

bool TextComponent::contains(Vector2f point) const
{
	return txt.getGlobalBounds().contains(point);
}

void TextComponent::tryClick(Vector2f mousePos, EntityID entityId, World& world)
{
	if (contains(mousePos) && onClick) 
	{
		onClick(entityId, world);
	}
}

void TextComponent::tryHover(Vector2f mousePos, EntityID entityId, World& world) {
	if (contains(mousePos)&&isHover)
		txt.setFillColor(Color::Cyan);
	else
		txt.setFillColor(originColor);
}
