#include "../../header/GameStates/MainMenu.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/GameStates/Credit.h"
#include "../../header/GameStates/Help.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Managers/World.h"
#include "../../header/GameStates/Setting.h"
#include "../../header/GameStates/Lobby.h"

void MainMenu::handleEvent(World& world, sf::Event& event)
{
    auto entities0 = world.getEntitiesWithComponent<TextComponent>();
    auto entities1 = world.getEntitiesWithComponent<SpriteComponent>();

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });

        for (EntityID e : entities0)
        {
            auto& txtComp = world.getComponent<TextComponent>(e);
            if (txtComp.tryClick(mousePos, e, world))
            {
                return;
            }
        }
        for (EntityID e : entities1)
        {
            if (!world.hasComponent<SpriteComponent>(e)) continue;

            auto& spriteComp = world.getComponent<SpriteComponent>(e);
            if (spriteComp.tryClick(mousePos, e, world))
            {
                return;
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseMove.x, event.mouseMove.y });

        for (EntityID e : entities0)
        {
            auto& txtComp = world.getComponent<TextComponent>(e);
            txtComp.tryHover(mousePos, e, world);
        }
    }
}


void MainMenu::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);
}

void MainMenu::onEnter(World& world)
{
    std::cout << "[MainMenu] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    MusicComponent musicComp(musicPath);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/MenuV3.png";
    SpriteComponent spriteComp(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp);
    
    //bgm
    if ((world.getSystem<MusicSystem>()->getEntity()).size() == 0) 
    {
        EntityID bgm = world.createEntity();
        registerEntity(bgm);
        world.addComponent(bgm, musicComp);
    }


    //Text
    EntityID title = world.createEntity();
    registerEntity(title);
    const string str0 = "Amelia: The Journey";
    const string fontPath0 = "assets/Font/Pixel Game.otf";
    TextComponent textComp0(str0, 180, fontPath0, Color::Yellow, { 1920 / 2, 330 }, false, sf::Color::Black, 15.f);
    world.addComponent(title, textComp0);

    EntityID playTxt = world.createEntity();
    registerEntity(playTxt);
    const string str1 = "Play";
    const string fontPath1 = "assets/Font/Minecraft-Regular.otf";
    TextComponent textComp1(str1, 70, fontPath1, Color::White, { 1920 / 2, 460 + 80 }, true, sf::Color::Black, 5.f);
    textComp1.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Play Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            world.setState(std::make_unique<Lobby>());
        };
    world.addComponent(playTxt, soundComp);
    world.addComponent(playTxt, textComp1);

    EntityID continueTxt = world.createEntity();
    registerEntity(continueTxt);
    const string str2 = "Continue";
    TextComponent textComp2(str2, 70, fontPath1, Color::White, { 1920 / 2, 360 + 50 + 30 + 200 }, true, sf::Color::Black, 5.f);
    textComp2.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Continue Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
        };
    world.addComponent(continueTxt, soundComp);
    world.addComponent(continueTxt, textComp2);

    EntityID exitTxt = world.createEntity();
    registerEntity(exitTxt);
    const string str3 = "Exit";
    TextComponent textComp3(str3, 70, fontPath1, Color::White, { 1920 / 2, 360 + 3 * 100 + 2 * 30 + 20 }, true, sf::Color::Black, 5.f);
    textComp3.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.requestClose();
        };
    world.addComponent(exitTxt, soundComp);
    world.addComponent(exitTxt, textComp3);

    //button
    EntityID setting = world.createEntity();
    registerEntity(setting);
    const string settingPath = "assets/Icon/Settings/B_Button44.png";
    SpriteComponent spriteComp0(settingPath, { 1620.f, 980.f }, { 5.f, 5.f });
    spriteComp0.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Setting Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            world.setState(std::make_unique<Setting>());

        };
    world.addComponent(setting, soundComp);
    world.addComponent(setting, spriteComp0);

    EntityID help = world.createEntity();
    registerEntity(help);
    const string helpPath = "assets/Icon/Unlock/B_Button2.png";
    SpriteComponent spriteComp1(helpPath, { 1720.f, 980.f }, { 5.f, 5.f });
    spriteComp1.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Help Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            world.setState(std::make_unique<Help>());
        };
    world.addComponent(help, soundComp);
    world.addComponent(help, spriteComp1);

    EntityID credit = world.createEntity();
    registerEntity(credit);
    const string creditPath = "assets/Icon/Level/B_Button53.png";
    SpriteComponent spriteComp2(creditPath, { 1820.f, 980.f }, { 5.f, 5.f });
    spriteComp2.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Credit Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            world.setState(std::make_unique<Credit>());
        };
    world.addComponent(credit, soundComp);
    world.addComponent(credit, spriteComp2);
}

void MainMenu::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}