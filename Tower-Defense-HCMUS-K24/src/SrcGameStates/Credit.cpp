#include "../../header/GameStates/Credit.h"
#include "../../header/GameStates/MainMenu.h"

#include "../../header/Components/SoundComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"

void Credit::onEnter(World& world)
{
    std::cout << "[Credit] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string fontPath = "assets/Font/Minecraft-Regular.otf";
    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);

    sf::Color outlineColor(0, 0, 0);
    sf::Color textColor(230, 230, 230);
    float thickness = 2.f;

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/CreditBg.jpg";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 5.f, 5.f });
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

    //text
    EntityID title = world.createEntity();
    registerEntity(title);
    const string str0 = "CREDITS";
    TextComponent textComp0(str0, 80, fontPath, textColor, { 1920 / 2, 75 }, false, outlineColor, thickness);
    world.addComponent(title, textComp0);

    EntityID text0 = world.createEntity();
    registerEntity(text0);
    const string str1 = "A TEAM 2 PRODUCTION";
    TextComponent textComp1(str1, 60, fontPath, textColor, { 1920 / 2, 200 }, false, outlineColor, thickness);
    world.addComponent(text0, textComp1);

    EntityID text1 = world.createEntity();
    registerEntity(text1);
    const string str2 = "DESIGNED AND CREATED BY";
    TextComponent textComp2(str2, 40, fontPath, textColor, { 1920 / 2, 325 + 50 }, false, outlineColor, thickness);
    world.addComponent(text1, textComp2);

    EntityID text2 = world.createEntity();
    registerEntity(text2);
    const string str3 = "LUONG HUNG PHAT";
    TextComponent textComp3(str3, 40, fontPath, textColor, { 1920 / 2, 375 + 50 }, false, outlineColor, thickness);
    world.addComponent(text2, textComp3);

    EntityID text3 = world.createEntity();
    registerEntity(text3);
    const string str4 = "PROGRAMMING";
    TextComponent textComp4(str4, 40, fontPath, textColor, { 1920 / 2, 475 + 50 }, false, outlineColor, thickness);
    world.addComponent(text3, textComp4);

    EntityID text4 = world.createEntity();
    registerEntity(text4);
    const string str5 = "LUONG HUNG PHAT   NGUYEN NHUT HUY   NGUYEN THANH DAT";
    TextComponent textComp5(str5, 40, fontPath, textColor, { 1920 / 2, 525 + 50 }, false, outlineColor, thickness);
    world.addComponent(text4, textComp5);

    EntityID text5 = world.createEntity();
    registerEntity(text5);
    const string str6 = "ART";
    TextComponent textComp6(str6, 40, fontPath, textColor, { 1920 / 2, 625 + 50 }, false, outlineColor, thickness);
    world.addComponent(text5, textComp6);

    EntityID text6 = world.createEntity();
    registerEntity(text6);
    const string str7 = "NGUYEN THANH DAT";
    TextComponent textComp7(str7, 40, fontPath, textColor, { 1920 / 2, 675 + 50 }, false, outlineColor, thickness);
    world.addComponent(text6, textComp7);

    EntityID text7 = world.createEntity();
    registerEntity(text7);
    const string str8 = "UI DESIGN";
    TextComponent textComp8(str8, 40, fontPath, textColor, { 1920 / 2, 775 + 50 }, false, outlineColor, thickness);
    world.addComponent(text7, textComp8);

    EntityID text8 = world.createEntity();
    registerEntity(text8);
    const string str9 = "NGUYEN NHUT HUY   NGUYEN THANH DAT";
    TextComponent textComp9(str9, 40, fontPath, textColor, { 1920 / 2, 825 + 50 }, false, outlineColor, thickness);
    world.addComponent(text8, textComp9);

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