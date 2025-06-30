#include "../../header/GameStates/GameState.h"
#include "../../header/Managers/World.h"
#include "../../header/Systems/MusicSystem.h"
#include <iostream>

using std::cout;

void GameState::onEnter(World& world) 
{

}

void GameState::onExit(World& world) 
{
    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities) {
        auto& musicComp = world.getComponent<MusicComponent>(id);
        if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
            musicComp.music->stop();
        }
    }
    for (EntityID id : createdEntities)
    {
        world.destroyEntity(id);
    }
    createdEntities.clear();
    cout << "Exit state and free memory successfully.\n";
}