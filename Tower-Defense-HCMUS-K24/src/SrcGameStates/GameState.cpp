#include "../../header/GameStates/GameState.h"
#include "../../header/Managers/World.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Systems/SpriteRenderSystem.h"
#include "../../header/Systems/InitializeProjectile.h"
#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Systems/TowerSystem.h"
#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Systems/InitializeEnemy.h"

#include <iostream>

using std::cout;

void GameState::onEnter(World& world)
{

}

void GameState::onExit(World& world)
{
    // Destroy all entities created by this state (except music)
    for (EntityID id : createdEntities)
    {
        if (world.hasComponent<MusicComponent>(id))
            continue;
        world.destroyEntity(id);
    }
    createdEntities.clear();
    cout << "Exit state and free memory successfully.\n";
}


