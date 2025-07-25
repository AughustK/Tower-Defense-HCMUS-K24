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
#include "../../header/Components/ProjectileComponent.h"
#include "../../header/Components/TowerComponent.h"


class GamePlay : public GameState 
{
private:
    //void spawnInitialEntities(World& world);
    //void spawnWave(World& world);
    bool isPlacingTower = false;
    TowerComponent::TowerType placingType = TowerComponent::TowerType::Archer;
    int placingLevel = 0;

    std::string                 mapFilename;
    std::vector<sf::Vector2f>   pathWaypoints;



    // enemy‐wave control
    std::vector<int>            waveSizes = { 8, 10, 1 };
    float                       waveInterval = 6.0f;   // secs between waves
    int                         currentWave = 0;
    float                       spawnTimer = 0.f;

    int enemiesToSpawn = 0;
    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 1.2f; // seconds between enemies
    std::vector<sf::Vector2f> currentWavePath;



    //money control
    int money = 0;
    int moneyPerWave = 100;


public:
    // construct by map filename or by index
    explicit GamePlay(const std::string& mapFilename);

    // State interface
    void onEnter(World& world) override;
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
    void onExit(World& world) override;
    void spawnTowerIcons(World& world);
	void spawnWave(World& world);
    void spawnInitialEntities(World& world);
};
