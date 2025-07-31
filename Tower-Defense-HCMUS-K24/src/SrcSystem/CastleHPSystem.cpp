#include <SFML/Graphics.hpp>
#include "../../header/Systems/System.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/CastleHPComponent.h"
#include "../../header/Systems/CastleHPSystem.h"
#include "../../header/GameStates/Defeat.h"

void CastleHPSystem::render(World& world)
{
    for (EntityID entity : entities)
    {
        auto& hpComp = world.getComponent<CastleHPComponent>(entity);

        world.window.draw(hpComp.border);
        world.window.draw(hpComp.fill);
        world.window.draw(hpComp.hpText);
    }
}

void CastleHPSystem::update(float deltaTime) {

}

void CastleHPSystem::update(World& world, int dmg) {
    std::cout << "[CastleHPSystem] Decreasing castle HP\n";
    for (EntityID entity : entities)
    {
        auto& hpComp = world.getComponent<CastleHPComponent>(entity);
        hpComp.setHP(hpComp.currentHP - dmg);
    }
}
