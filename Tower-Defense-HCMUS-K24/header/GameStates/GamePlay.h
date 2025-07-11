// header/GameStates/GamePlay.h
#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../../header/GameStates/GameState.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/PathfindingComponent.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/PositionComponent.h"


class GamePlay : public GameState 
{
private:
    void spawnInitialEntities(World& world);
    void spawnWave(World& world);

    std::string                 mapFilename;
    std::vector<sf::Vector2f>   pathWaypoints;
    std::vector<EntityID>   createdEntities;

    // enemy‐wave control
    std::vector<int>            waveSizes = { 5, 10, 15 };
    float                       waveInterval = 5.f;   // secs between waves
    int                         currentWave = 0;
    float                       spawnTimer = 0.f;

public:
    // construct by map filename or by index
    explicit GamePlay(const std::string& mapFilename);

    // State interface
    void onEnter(World& world) override;
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
};
