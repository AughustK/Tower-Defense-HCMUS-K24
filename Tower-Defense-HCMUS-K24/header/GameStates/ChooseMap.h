#pragma once

#include "GameState.h"
#include "../Managers/World.h"
#include "GamePlay.h"
#include "../Systems/SpriteRenderSystem.h"
#include "../Components/Difficulty.h"
#include <iostream>
#include <string>

class ChooseMap : public GameState
{
private:
    DifficultyLevel currentDifficulty = DifficultyLevel::Easy;

public:
    ChooseMap() = default;
    ChooseMap(DifficultyLevel difficulty) : currentDifficulty(difficulty) {}
    
	void handleEvent(World& world, sf::Event& event) override;
	void update(World& world, float dt) override;
	void render(World& world, sf::RenderWindow& window) override;
	void onEnter(World& world) override;
}; 
