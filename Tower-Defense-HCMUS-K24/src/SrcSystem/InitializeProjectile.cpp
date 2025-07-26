#include "../../header/Systems/InitializeProjectile.h"
#include "../../header/Components/VelocityComponent.h"

void ProjectilePoolSystem::initPool(World& world, std::size_t count)
{
    static sf::Texture arrowTexture;
    static bool   isLoaded = [] 
    {
        if (!arrowTexture.loadFromFile("assets/Heroes/Archer/Arrow.png")) {
            throw std::runtime_error("Failed to load Arrow.png");
        }
        return true;
    }();

    createdProjectile.clear();
    createdProjectile.reserve(count);

    ProjectileComponent proj{};
    proj.x = proj.y = -100.f;                // off-screen
    proj.tag = ProjectileComponent::ProjectileType::Arrow;   // default type
    SpriteComponent sprite{ arrowTexture, sf::Vector2f{43,45}, sf::Vector2f{5,5} };

    for (std::size_t i = 0; i < count; ++i)
    {
        auto e = world.createEntity();


        world.addComponent(e, sprite);
        world.addComponent(e, proj);
        world.addComponent(e, VelocityComponent{ 0.f, 0.f });


        PositionComponent pos{ proj.x, proj.y, PositionComponent::Type::Projectile };
        world.addComponent(e, pos);
        createdProjectile.push_back(e);
    }
    nextIndex = 0;
}

EntityID ProjectilePoolSystem::spawn(World& world, float startX, float startY, const sf::Vector2f& velocity, ProjectileComponent::ProjectileType type)
{
    EntityID e = createdProjectile[nextIndex];
    nextIndex = (nextIndex + 1) % createdProjectile.size();

    //reset projectile component
    auto& proj = world.getComponent<ProjectileComponent>(e);
    proj.x = startX;
    proj.y = startY;
    proj.tag = type;
    
    proj.x = velocity.x;
    proj.y = velocity.y;

    //reset position component
    auto& pos = world.getComponent<PositionComponent>(e);
    pos.x = startX;
    pos.y = startY;

    //move the sprite
    auto& spriteComp = world.getComponent<SpriteComponent>(e);
    spriteComp.sprite.setPosition(startX, startY);
    return e;
}
