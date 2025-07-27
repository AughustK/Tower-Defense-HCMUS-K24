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
	sf::Clock lastSlideClock;
	bool reachedLastSlide = false;
	bool shouldExit = false;
	vector<string> slides;
	vector<string> voice;
	int currentSlide = 0;
	EntityID slideBoxEntity;
	EntityID voiceEntityID;
public:
	void handleEvent(World& world, sf::Event& event) override;
	void update(World& world, float dt) override;
	void render(World& world, sf::RenderWindow& window) override;
	void onEnter(World& world) override;
	void updateSlide(World& world);
	void onExit(World& world) override;
};