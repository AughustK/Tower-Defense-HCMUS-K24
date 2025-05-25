#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <vector>

class Entity
{
public:
	Entity() {};
	~Entity() {};

	struct PhysicData
	{
		enum Shape
		{
			Circle,
			Rectangle
		};

		enum Type
		{
			Static,
			Dynamic
		};

		Shape m_eShape;
		Type m_eType;
		sf::Vector2f m_vVelocity;
		sf::Vector2f m_vPosition;    // Optional, might be used 
	};

	void SetTexture(const sf::Texture& texture)
	{
		m_sprite.setTexture(texture);
	}

	void SetPosition(const sf::Vector2f& position)
	{
		m_sprite.setPosition(position);
	}

	void SetScale(const sf::Vector2f& scale)
	{
		m_sprite.setScale(scale);
	}

	void SetOrigin(const sf::Vector2f& origin)
	{
		m_sprite.setOrigin(origin);
	}
private:
	sf::Sprite m_sprite;


};

#endif // ENTITY_H
