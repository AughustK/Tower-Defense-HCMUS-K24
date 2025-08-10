#pragma once
#include "GameState.h"
#include "../Managers/World.h"
#include <vector>
#include <string>

constexpr int VISIBLE_COUNT = 5;

class Continue : public GameState {
    std::vector<std::string> saveFiles;

    std::vector<EntityID> fileTextEntities;  
    std::vector<EntityID> fileDeleteEntities; 
    std::vector<EntityID> controlEntities;

    int scrollOffset = 0; 

    void loadSaveFiles();
    void spawnTextOptions(World& world); 
    void spawnControlButtons(World& world); 
    void clampScroll();

public:
    void onEnter(World& world) override;
    void update(World& world, float dt) override;
    void handleEvent(World& world, sf::Event& event) override;
    void render(World& world, sf::RenderWindow& window) override;
};