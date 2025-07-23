#pragma once

#include "../../header/GameStates/GameState.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/UITextComponent.h"   
#include "../../header/Components/UISpriteComponent.h" 
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/ClickComponent.h"
#include "../../header/Components/SoundComponent.h"    
#include "../../header/Components/MusicComponent.h"    

#include <iostream>
#include <string>
#include <memory> 

class PauseMenu : public GameState
{
public:
    void onEnter(World& world) override;
    void handleEvent(World& world, sf::Event& event) override;
    void update(World& world, float dt) override;
    void render(World& world, sf::RenderWindow& window) override;
    void onExit(World& world) override;
};