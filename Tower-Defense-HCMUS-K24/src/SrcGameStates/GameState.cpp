#include "../../header/GameStates/GameState.h"
#include "../../header/Managers/World.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Systems/SpriteRenderSystem.h"

#include <iostream>

using std::cout;

void GameState::onEnter(World& world)
{

}

void GameState::onExit(World& world)
{
    for (EntityID id : createdEntities)
    {
        if (world.hasComponent<MusicComponent>(id))
            continue;
        world.destroyEntity(id);
    }
    createdEntities.clear();
    world.getSystem<SpriteRenderSystem>()->clear();
    cout << "Exit state and free memory successfully.\n";
}


