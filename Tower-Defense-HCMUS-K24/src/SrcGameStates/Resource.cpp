#include "../../header/Managers/World.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/GameStates/Lobby.h"
#include "../../header/GameStates/Resource.h"
#include "../../header/GameStates/ChooseMap.h"


void Resource::onEnter(World& world)
{
    std::cout << "[Resource] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    MusicComponent musicComp(musicPath);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/ResourceBg.png";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);
    world.addComponent(background, musicComp);

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
            world.setState(std::make_unique<Lobby>());
        };
    world.addComponent(exitButton, soundComp);
    world.addComponent(exitButton, spriteComp1);
}

void Resource::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void Resource::handleEvent(World& world, sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
        {
            auto& sc = world.getComponent<SpriteComponent>(e);
            if (sc.tryClick(mousePos, e, world))
                return;
        }
    }
}

void Resource::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}