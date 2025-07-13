#include "../../header/Managers/World.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/GameStates/ChooseMap.h"
#include "../../header/GameStates/Lobby.h"


void ChooseMap::onEnter(World& world)
{
    std::cout << "[ChooseMap] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    MusicComponent musicComp(musicPath);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/ChooseMap.png";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);

    if ((world.getSystem<MusicSystem>()->getEntity()).size() == 0) {
        EntityID bgm = world.createEntity();
        registerEntity(bgm);
        world.addComponent(bgm, musicComp);
    }

    //hellmap flag
    EntityID blackFlag = world.createEntity();
    registerEntity(blackFlag);
    const string blackFlagPath = "assets/Bg/BlackFlag.png";
    SpriteComponent spriteComp2(blackFlagPath, { 1100.f, 80.f }, { 0.5f, 0.5f });
    spriteComp2.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][HellMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            const string hellMPath = "HellMap";
            world.setState(std::make_unique<GamePlay>(hellMPath));
        };
    world.addComponent(blackFlag, soundComp);
    world.addComponent(blackFlag, spriteComp2);

    //ice map
    EntityID blueFlag = world.createEntity();
    registerEntity(blueFlag);
    const string blueFlagPath = "assets/Bg/BlueFlag.png";
    SpriteComponent spriteComp3(blueFlagPath, { 1380.f, 400.f }, { 0.5f, 0.5f });
    spriteComp3.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][IcelMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            const string iceMPath = "IceMap";
            world.setState(std::make_unique<GamePlay>(iceMPath));
        };
    world.addComponent(blueFlag, soundComp);
    world.addComponent(blueFlag, spriteComp3);

    //paradise map
    EntityID whiteFlag = world.createEntity();
    registerEntity(whiteFlag);
    const string whiteFlagPath = "assets/Bg/WhiteFlag.png";
    SpriteComponent spriteComp4(whiteFlagPath, { 1100.f, 700.f }, { 0.5f, 0.5f });
    spriteComp4.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][ParadiselMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            const string paraMPath = "ParadiseMap";
            world.setState(std::make_unique<GamePlay>(paraMPath));
        };
    world.addComponent(whiteFlag, soundComp);
    world.addComponent(whiteFlag, spriteComp4);

    //firemap
    EntityID redFlag = world.createEntity();
    registerEntity(redFlag);
    const string redFlagPath = "assets/Bg/redFlag.png";
    SpriteComponent spriteComp5(redFlagPath, { 400.f, 250.f }, { 0.5f, 0.5f });
    spriteComp5.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][FireMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            const string fireMPath = "FireMap";
            world.setState(std::make_unique<GamePlay>(fireMPath));
        };
    world.addComponent(redFlag, soundComp);
    world.addComponent(redFlag, spriteComp5);

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

void ChooseMap::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void ChooseMap::handleEvent(World& world, sf::Event& event)
{
    auto entities = world.getEntitiesWithComponent<SpriteComponent>();
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });
        for (EntityID e : entities)
        {
            auto& spriteComp = world.getComponent<SpriteComponent>(e);
            if (spriteComp.tryClick(mousePos, e, world))
            {
                return;
            }
        }
        for (EntityID e : world.getEntitiesWithComponent<ClickComponent>())
        {
            auto& clickComp = world.getComponent<ClickComponent>(e);
            if (clickComp.tryClick(mousePos, e, world)) {
                return;   
            }
        }
    }
}

void ChooseMap::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}