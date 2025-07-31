#include "../../header/Systems/InitializeProjectile.h"
#include "../../header/Components/VelocityComponent.h"
#include "../../header/Components/CircleComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Components/TowerDef.h"

inline const std::string& getProjectileSpritePath(ProjectileComponent::ProjectileType type) {
    static const std::unordered_map<ProjectileComponent::ProjectileType, std::string> paths = {
        { ProjectileComponent::ProjectileType::Arrow,       "assets/Heroes/Shooter/ShooterNormal/5 Bullets/9.png" },
        { ProjectileComponent::ProjectileType::MagicCircle, "assets/Heroes/Shooter/ShooterNormal/5 Bullets/3.png" },
        { ProjectileComponent::ProjectileType::CannonBall,  "assets/Heroes/Shooter/ShooterNormal/5 Bullets/10.png" }
    };
    return paths.at(type);
}

void ProjectilePoolSystem::initPool(World& world, std::size_t count)
{
    pool.clear();
    pool.reserve(count);
    for (size_t i = 0; i < count; ++i) 
    {
        EntityID e = world.createEntity();
        world.addComponent(e, PositionComponent());
        world.addComponent(e, VelocityComponent());
        world.addComponent(e, ProjectileComponent());
        world.addComponent(e, CircleComponent());
        world.addComponent(e, SpriteComponent());
        pool.push_back(e);
    }
    nextIndex = 0;
}

EntityID ProjectilePoolSystem::spawn(World& world,
    float startX,
    float startY,
    const sf::Vector2f& velocity,
    ProjectileComponent::ProjectileType type,
    int level,
    EntityID owner) {

    //get entity from pool
    EntityID e = pool[nextIndex];
    nextIndex = (nextIndex + 1) % pool.size();

    // Position
    auto& pos = world.getComponent<PositionComponent>(e);
    pos.x = startX;
    pos.y = startY;

    // Velocity
    auto& vel = world.getComponent<VelocityComponent>(e);
    vel.x = velocity.x;
    vel.y = velocity.y;

    // ProjectileComponent
    auto& pc = world.getComponent<ProjectileComponent>(e);
    pc.x = startX;
    pc.y = startY;
    pc.tag = type;
    pc.owner = owner;
    pc.arrowLength = (type == ProjectileComponent::ProjectileType::Arrow)
        ? TOWER_DEFS[static_cast<int>(world.getComponent<TowerComponent>(owner).type)].projectileLength
        : 0.0f;

    // CircleComponent
    auto& cc = world.getComponent<CircleComponent>(e);
    cc.x = startX;
    cc.y = startY;
    cc.tag = CircleComponent::CollisionType::Projectile;
    cc.radius = TOWER_DEFS[static_cast<int>(world.getComponent<TowerComponent>(owner).type)].projectileRadius;

    auto& sprite = world.getComponent<SpriteComponent>(e);
    const std::string& path = getProjectileSpritePath(type);
    sprite.setTxt(path);
    sprite.sprite.setPosition(startX, startY);

    float scale = TOWER_DEFS[static_cast<int>(world.getComponent<TowerComponent>(owner).type)].scale[level];
    if(world.getComponent<TowerComponent>(owner).type == TowerComponent::TowerType::Archer) 
    {
        scale = -0.5f;
	}
    sprite.sprite.setScale(scale + 2.0f, scale + 2.0f);

    return e;
}

void ProjectilePoolSystem::clearPool(World& world) {
    for (int i = 0; i < pool.size(); i++) {
        std::cout << "[ProjectileSystem] Destroying " << pool[i] << endl;
        world.destroyEntity(pool[i]);
    }
    pool.clear();
}

void ProjectilePoolSystem::hideUsedProj(World& world, EntityID en) {
    if (world.hasComponent<PositionComponent>(en)) {
        auto& pos = world.getComponent<PositionComponent>(en);
        pos.x = -1000;
        pos.y = -1000;
    }

    if (world.hasComponent<VelocityComponent>(en)) {
        auto& vel = world.getComponent<VelocityComponent>(en);
        vel.x = 0.0f;
        vel.y = 0.0f;
    }

    if (world.hasComponent<ProjectileComponent>(en)) {
        auto& proj = world.getComponent<ProjectileComponent>(en);
        proj.x = -1000;
        proj.y = -1000;
    }

    if (world.hasComponent<CircleComponent>(en)) {
        auto& cc = world.getComponent<CircleComponent>(en);
        cc.x = -1000;
        cc.y = -1000;
    }

    if (world.hasComponent<SpriteComponent>(en)) {
        auto& sprite = world.getComponent<SpriteComponent>(en);
        sprite.sprite.setPosition(-1000, -1000);
    }
}