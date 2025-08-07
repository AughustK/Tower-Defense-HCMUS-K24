#include "../../header/GameStates/Credit.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/Systems/MusicSystem.h"

void Credit::onEnter(World& world)
{
    std::cout << "[Credit] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string fontPath = "assets/Font/Minecraft-Regular.otf";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);

    sf::Color outlineColor(0, 0, 0);
    sf::Color textColorWhite(230, 230, 230);
    sf::Color textColorPink(255, 64, 129);
    float thickness = 2.f;

    // Background
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/CreditBg.jpg";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 5.f, 5.f });
    world.addComponent(background, spriteComp0);

    // Exit Button
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

    // Title
    int y = 80;
    int flag = false;
    auto addText = [&](const string& str, int fontSize, int yPos) {
        EntityID e = world.createEntity();
        registerEntity(e);
        if (flag == false)
        {
            TextComponent tc(str, fontSize, fontPath, textColorPink, { 1920 / 2, static_cast<float>(yPos) }, false, outlineColor, thickness);
            world.addComponent(e, tc);
            flag = true;
        }
        else
        {
            TextComponent tc(str, fontSize, fontPath, textColorWhite, { 1920 / 2, static_cast<float>(yPos) }, false, outlineColor, thickness);
            world.addComponent(e, tc);
            flag = false;
        }
        };

    addText("CREDITS", 80, y);               y += 100;
    addText("A TEAM 2 (24C11 - HCMUS) PRODUCTION", 60, y); y += 100;

    // Project Lead
    addText("PROJECT LEAD", 40, y);          y += 45;
    addText("Luong Hung Phat", 35, y);       y += 65;

    // Game Design
    addText("GAME DESIGN", 40, y);           y += 45;
    addText("Luong Hung Phat   Nguyen Nhut Huy   Nguyen Thanh Dat   Nguyen Anh Khoi   Nguyen Quoc Duong", 35, y); y += 65;

    // Programming
    addText("PROGRAMMING", 40, y);           y += 45;
    addText("Luong Hung Phat   Nguyen Nhut Huy   Nguyen Thanh Dat   Nguyen Anh Khoi   Nguyen Quoc Duong", 35, y); y += 65;

    // Art / UI Design
    addText("ART / UI DESIGN", 40, y);       y += 45;
    addText("Luong Hung Phat   Nguyen Nhut Huy   Nguyen Thanh Dat", 35, y); y += 65;

    // Animation
    addText("ANIMATION", 40, y);             y += 45;
    addText("Nguyen Anh Khoi", 35, y);       y += 65;

    // Sound / Music
    addText("SOUND / MUSIC", 40, y);         y += 45;
    addText("Luong Hung Phat   Nguyen Nhut Huy", 35, y); y += 65;

    // Story / Writing
    addText("STORY / WRITING", 40, y);       y += 45;
    addText("Nguyen Thanh Dat", 35, y);
}


void Credit::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);
}

void Credit::handleEvent(World& world, sf::Event& event)
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

void Credit::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}