#pragma once
#include "GameState.h"
#include "../Managers/World.h"
#include <vector>
#include <string>

class Continue : public GameState {
    std::vector<std::string> saveFiles;
    std::vector<EntityID> textEntities;

    void loadSaveFiles();
    void spawnTextOptions(World& world);
    void clearUI(World& world);

public:
    void onEnter(World& world) override;
    void onExit(World& world) override;
    void update(World& world, float dt) override;
    void handleEvent(World& world, sf::Event& event) override;
    void render(World& world, sf::RenderWindow& window) override;
};
