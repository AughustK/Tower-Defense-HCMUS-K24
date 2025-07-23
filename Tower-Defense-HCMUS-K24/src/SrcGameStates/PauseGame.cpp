#include "../../header/GameStates/PauseGame.h"
#include "../../header/GameStates/MainMenu.h"  
#include "../../header/GameStates/GamePlay.h"

#include "../../header/Systems/PhysicSystem.h"
#include "../../header/Systems/InitializeEnemy.h" 
#include "../../header/Systems/InitializeProjectile.h" 
#include "../../header/Systems/PathFindingSystem.h"
#include "../../header/Systems/CollisionSystem.h"
#include "../../header/Systems/TowerSystem.h" 
#include "../../header/Systems/UISliderSystem.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"

void PauseMenu::onEnter(World& world)
{
    //Pause button
    std::cout << "[PauseMenu] Entered pause state." << std::endl;

    EntityID pausedTextEntity = world.createEntity();
    registerEntity(pausedTextEntity);
    TextComponent pausedText(
        "PAUSED",
        72,
        "assets/Font/Pixel Game.otf",
        sf::Color::White,
        sf::Vector2f(world.window.getSize().x / 2.f, world.window.getSize().y / 2.f - 150.f),
        false,
        sf::Color::Black,
        2.0f
    );
    pausedText.txt.setOrigin(pausedText.txt.getLocalBounds().width / 2.f, pausedText.txt.getLocalBounds().height / 2.f);
    world.addComponent(pausedTextEntity, pausedText);

    // Resume button
    EntityID resumeButton = world.createEntity();
    registerEntity(resumeButton);
    TextComponent resumeText(
        "Resume",
        48,
        "assets/Font/Pixel Game.otf",
        sf::Color::Yellow,
        sf::Vector2f(world.window.getSize().x / 2.f, world.window.getSize().y / 2.f),
        true,
        sf::Color::Black,
        1.5f
    );
    resumeText.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[PauseMenu] Resuming GamePlay." << std::endl;
            world.setState(std::make_unique<GamePlay>("current_map.txt"));
        };
    world.addComponent(resumeButton, resumeText);

    // Quit button
    EntityID quitButton = world.createEntity();
    registerEntity(quitButton);
    TextComponent quitText(
        "Quit",
        48,
        "assets/Font/Pixel Game.otf",
        sf::Color::Yellow,
        sf::Vector2f(world.window.getSize().x / 2.f, world.window.getSize().y / 2.f + 100.f),
        true,
        sf::Color::Black,
        1.5f
    );
    quitText.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[PauseMenu] Quitting to MainMenu." << std::endl;
            world.setState(std::make_unique<MainMenu>());
        };
    world.addComponent(quitButton, quitText);

   
}

void PauseMenu::handleEvent(World& world, sf::Event& event)
{
    auto entities = world.getEntitiesWithComponent<TextComponent>();

    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseMove.x, event.mouseMove.y });

        for (EntityID e : entities)
        {
            auto& txtComp = world.getComponent<TextComponent>(e);
            txtComp.tryHover(mousePos, e, world);
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });

        for (EntityID e : entities)
        {
            auto& txtComp = world.getComponent<TextComponent>(e);
            if (txtComp.tryClick(mousePos, e, world))
            {
                return;
            }
        }
    }

    //Esc key to resume game
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        std::cout << "[PauseMenu] Escape pressed! Resuming GamePlay." << std::endl;
        world.setState(std::make_unique<GamePlay>("map1.txt"));
    }
}

void PauseMenu::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}

void PauseMenu::render(World& world, sf::RenderWindow& window)
{
    auto spriteRenderSystem = world.getSystem<SpriteRenderSystem>();
    if (spriteRenderSystem && spriteRenderSystem->isEnabled()) 
    {
        spriteRenderSystem->render(world);
    }

    auto textRenderSystem = world.getSystem<TextRenderSystem>();
    if (textRenderSystem && textRenderSystem->isEnabled())
    {
        textRenderSystem->render(world);
    }

    auto sliderSystem = world.getSystem<SliderSystem>();
    if (sliderSystem && sliderSystem->isEnabled())
    {
        sliderSystem->render(world);
    }
}

void PauseMenu::onExit(World& world)
{
    std::cout << "[PauseMenu] Exited pause state." << std::endl;
    for (EntityID entity : createdEntities)
    {
        world.destroyEntity(entity);
    }
    createdEntities.clear();

    //Enable system in gameplay
    if (world.getSystem<PhysicSystem>()) world.getSystem<PhysicSystem>()->enable();
    if (world.getSystem<EnemySpawnSystem>()) world.getSystem<EnemySpawnSystem>()->enable();
    if (world.getSystem<ProjectilePoolSystem>()) world.getSystem<ProjectilePoolSystem>()->enable();
    if (world.getSystem<PathFollowingSystem>()) world.getSystem<PathFollowingSystem>()->enable();
    if (world.getSystem<CollisionSystem>()) world.getSystem<CollisionSystem>()->enable();
    if (world.getSystem<TowerSystem>()) world.getSystem<TowerSystem>()->enable();
    if (world.getSystem<SoundSystem>()) world.getSystem<SoundSystem>()->enable();
    if (world.getSystem<MusicSystem>()) world.getSystem<MusicSystem>()->enable();
}