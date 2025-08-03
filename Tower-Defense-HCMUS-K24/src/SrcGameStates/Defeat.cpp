#include "../../header/GameStates/Defeat.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/GameStates/ChooseMap.h"
#include "../../header/Systems/MusicSystem.h"

void Defeat::onEnter(World& world) {
    std::cout << "[Defeat] onEnter called\n";

    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities)
    {
        auto& musicComp = world.getComponent<MusicComponent>(id);
        if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
            musicComp.music->stop();
        }
        world.destroyEntity(id);
    }

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string fontPath = "assets/Font/Nightmare Codehack.otf";
    const string musicPath = "assets/SFX/Music/Defeat.mp3";
    SoundComponent soundComp(soundPath, false);
    MusicComponent musicComp(musicPath);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);

    sf::Color outlineColor(0, 0, 0);
    sf::Color textColor = sf::Color::Yellow;
    float thickness = 8.f;

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/DefeatBg.png";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);

    //button
    EntityID homeButton = world.createEntity();
    registerEntity(homeButton);
    const string buttonPath1 = "assets/Icon/Home/B_Button16.png";
    SpriteComponent spriteComp1(buttonPath1, { 681.f, 655.f }, { 10.f, 10.f });
    spriteComp1.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Home Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<MainMenu>());
        };
    world.addComponent(homeButton, soundComp);
    world.addComponent(homeButton, spriteComp1);

    EntityID retryButton = world.createEntity();
    registerEntity(retryButton);
    const string buttonPath2 = "assets/Icon/Restart/B_Button70.png";
    SpriteComponent spriteComp2(buttonPath2, { 1043.f, 655.f }, { 10.f, 10.f });
    spriteComp2.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Retry Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            world.setState(std::make_unique<ChooseMap>());
        };
    world.addComponent(retryButton, soundComp);
    world.addComponent(retryButton, spriteComp2);

    //victory bgm
    EntityID bgm = world.createEntity();
    registerEntity(bgm);
    world.addComponent(bgm, musicComp);
}

void Defeat::render(World& world, sf::RenderWindow& window) {
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);
}

void Defeat::handleEvent(World& world, sf::Event& event)
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
    }
}

void Defeat::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}

void Defeat::onExit(World& world) {
    auto musicEntities = world.getEntitiesWithComponent<MusicComponent>();
    for (EntityID id : musicEntities)
    {
        auto& musicComp = world.getComponent<MusicComponent>(id);
        if (musicComp.music && musicComp.music->getStatus() == sf::Music::Playing) {
            musicComp.music->stop();
        }
    }
    for (EntityID id : createdEntities)
    {
        world.destroyEntity(id);
    }
    createdEntities.clear();
    world.getSystem<SpriteRenderSystem>()->clear();
    cout << "Exit state and free memory successfully.\n";
}