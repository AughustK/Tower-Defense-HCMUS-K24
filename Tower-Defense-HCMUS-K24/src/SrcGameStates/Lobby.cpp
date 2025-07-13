#include "../../header/Managers/World.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/GameStates/Lobby.h"
#include "../../header/GameStates/Setting.h"
#include "../../header/GameStates/ChooseMap.h"
#include "../../header/GameStates/Resource.h"


void Lobby::onEnter(World& world)
{
    std::cout << "[Lobby] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/Lobby.jpg";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);

    //button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    const string buttonPath = "assets/Icon/Left/B_Button68.png";
    SpriteComponent spriteComp1(buttonPath, { 0.f, 0.f }, { 5.f, 5.f });
    spriteComp1.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<MainMenu>());
        };
    world.addComponent(exitButton, soundComp);
    world.addComponent(exitButton, spriteComp1);

    //map - zone
    EntityID mapZone = world.createEntity();
    registerEntity(mapZone);
    sf::FloatRect mapRect{ 435.f, 65.f, 220.f, 230.f };
    SoundComponent zoneSound(soundPath, false);
    zoneSound.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    world.addComponent(mapZone, zoneSound);
    world.addComponent<ClickComponent>(mapZone, ClickComponent{
        mapRect,
        [soundPath, mapZone](EntityID entityId, World& world)
        {
            std::cout << "[Lobby][Map Zone] Clicked!\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<ChooseMap>());
        }
    });

    //resource item - zone
    EntityID resourceZone = world.createEntity();
    registerEntity(resourceZone);
    sf::FloatRect resourceRect{ 870.f, 65.f, 690.f, 440.f };
    SoundComponent zoneSound1(soundPath, false);
    zoneSound1.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    world.addComponent(resourceZone, zoneSound1);
    world.addComponent<ClickComponent>(resourceZone, ClickComponent{
        resourceRect,
        [soundPath, resourceZone](EntityID entityId, World& world)
        {
            std::cout << "[Lobby][Resource Zone] Clicked!\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<Resource>());
        }
        });
}

void Lobby::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void Lobby::handleEvent(World& world, sf::Event& event)
{

    if (event.type == sf::Event::Resized)
    {
        sf::FloatRect viewArea(
            0.f, 0.f,
            float(event.size.width),
            float(event.size.height)
        );
        world.window.setView(sf::View(viewArea));
    }


    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = world.window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
        {
            auto& sc = world.getComponent<SpriteComponent>(e);
            if (sc.tryClick(mousePos, e, world))
                return;  
        }

        for (EntityID e : world.getEntitiesWithComponent<ClickComponent>()) 
        {
            auto& cc = world.getComponent<ClickComponent>(e);
            if (cc.tryClick(mousePos, e, world)) 
            {
                return;  
            }
        }
    }
}

void Lobby::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}