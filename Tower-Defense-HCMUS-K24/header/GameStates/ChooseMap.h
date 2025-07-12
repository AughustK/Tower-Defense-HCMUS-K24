#pragma once

#include "GameState.h"
#include "../Managers/World.h"
#include "GamePlay.h"
#include "../Systems/SpriteRenderSystem.h"
#include <iostream>
#include <string>

class ChooseMap : public GameState
{
public:
	void handleEvent(World& world, sf::Event& event) override;
	void update(World& world, float dt) override;
	void render(World& world, sf::RenderWindow& window) override;
	void onEnter(World& world) override;
}; 
