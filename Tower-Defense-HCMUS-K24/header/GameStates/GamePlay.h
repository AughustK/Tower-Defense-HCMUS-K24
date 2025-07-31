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
#include "../Components/EnemyDef.h" // For DifficultyLevel

class GamePlay : public GameState 
{
    static int money;
private:
    bool isPlacingTower = false;
    TowerComponent::TowerType placingType = TowerComponent::TowerType::Archer;
    int placingLevel = 0;
    DifficultyLevel currentDifficulty = DifficultyLevel::Normal;

    std::string                 mapFilename;
    std::vector<sf::Vector2f>   pathWaypoints;


    // enemy‐wave control
    std::vector<int>            waveSizes = { 8, 10, 10, 1 };
    float                       waveInterval = 6.0f;   // secs between waves
    int                         currentWave = 0;
    float                       spawnTimer = 0.f;

    int enemiesToSpawn = 0;
    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 1.5f; // seconds between enemies
    std::vector<sf::Vector2f> currentWavePath;
    std::unordered_map<std::string, std::vector<sf::Vector2f>> validTowerSpotsPerMap;
    std::unordered_map<std::string, sf::Vector2f> castlePos;

    EntityID notificationEntity;
    bool notificationActive = false;
    float notificationTimer = 0.f;

    // Tower interaction variables
    EntityID selectedTower = INVALID_ENTITY;
    EntityID upgradeButton = INVALID_ENTITY;
    EntityID deleteButton = INVALID_ENTITY;
    std::vector<EntityID> towerOptionEntities; // Track all option entities
    bool towerOptionsVisible = false;

    // Tower interaction helper functions
    EntityID findTowerAtPosition(World& world, const sf::Vector2f& mousePos);
    void showTowerOptions(World& world, EntityID towerId, const sf::Vector2f& mousePos);
    void hideTowerOptions(World& world);
    void upgradeTower(World& world, EntityID towerId);
    void deleteTower(World& world, EntityID towerId);

	//Victory and Defeat conditions
    EntityID castleEntity;
    bool victoryTriggered = false;

public:
    // construct by map filename or by index
    explicit GamePlay(const std::string& mapFilename);
    GamePlay(const std::string& mapFilename, DifficultyLevel difficulty);

    // State interface
    void onEnter(World& world) override;
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
    void onExit(World& world) override;
    void spawnTowerIcons(World& world);
	void spawnWave(World& world);
    static void updateMoney(int g);
};
