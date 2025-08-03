
#include "../../header/Systems/EnemyHPSystem.h"
#include "../../header/Components/EnemyHPComponent.h"
#include "../../header/Components/PositionComponent.h"
#include "../../header/Components/HealthComponent.h"

void EnemyHPSystem::update(World& world, float dt) {
    auto& hpArray = world.getComponentArray<EnemyHPComponent>();
    auto& posArray = world.getComponentArray<PositionComponent>();
    auto& healthArray = world.getComponentArray<HealthComponent>();

    for (auto& [entity, _] : hpArray.getEntityToIndexMap()) {
        if (!posArray.containData(entity) || !healthArray.containData(entity)) continue;

        auto& visHP = hpArray.getData(entity);
        auto& pos = posArray.getData(entity);
        auto& logicHP = healthArray.getData(entity);

        visHP.setHP(logicHP.currentHealth);
        visHP.updatePosition({ pos.x, pos.y });
    }
}

void EnemyHPSystem::render(World& world) {
    auto& hpArray = world.getComponentArray<EnemyHPComponent>();
    for (auto& [entity, _] : hpArray.getEntityToIndexMap()) {
        auto& hp = hpArray.getData(entity);
        world.window.draw(hp.border);
        world.window.draw(hp.fill);
    }
}

void EnemyHPSystem::update(float deltaTime) {

}
