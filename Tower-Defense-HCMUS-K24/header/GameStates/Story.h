#pragma once
#pragma once

#include "GameState.h"
#include "../Managers/World.h"
#include "GamePlay.h"
#include "../Systems/SpriteRenderSystem.h"
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;


class Story : public GameState
{
private:
	bool shouldExit = false;
	EntityID slideBoxEntity;
	vector<string> slides;
	int currentSlide = 0;
public:
	void handleEvent(World& world, sf::Event& event) override;
	void update(World& world, float dt) override;
	void render(World& world, sf::RenderWindow& window) override;
	void onEnter(World& world) override;
	void updateSlide(World& world);
	void onExit(World& world) override;
};
