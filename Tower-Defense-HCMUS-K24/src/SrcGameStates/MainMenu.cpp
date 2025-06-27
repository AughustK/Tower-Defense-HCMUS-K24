#include "../../header/GameStates/MainMenu.h"

void MainMenu::handleEvent(World& world, sf::Event& event) 
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        auto entities = world.getEntitiesWithComponent<TextComponent>();
        for (EntityID e : entities)
        {
            auto& txtComp = world.getComponent<TextComponent>(e);
            txtComp.tryClick(mousePos, e, world);
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

    //bg
    EntityID background = world.createEntity();
    const string bgPath = "assets/Bg/MenuV2-1.png";
    SpriteComponent spriteComp(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp);

    //Text
    EntityID title = world.createEntity();
    const string str0 = "TOWER DEFENSE";
    const string fontPath0 = "assets/Font/KarmaFuture.ttf";
    TextComponent textComp0(str0, 140, fontPath0, Color::White, { 1920 / 2, 330 });
    world.addComponent(title, textComp0);

    EntityID playTxt = world.createEntity();
    const string str1 = "Play";
    const string fontPath1 = "assets/Font/Minecraft-Regular.otf";
    TextComponent textComp1(str1, 90, fontPath1, Color::White, { 1920 / 2, 460 + 50 });
    world.addComponent(playTxt, textComp1);

    EntityID continueTxt = world.createEntity();
    const string str2 = "Continue";
    TextComponent textComp2(str2, 90, fontPath1, Color::White, { 1920 / 2, 360 + 50 + 30 + 200 });
    world.addComponent(continueTxt, textComp2);

    EntityID exitTxt = world.createEntity();
    const string str3 = "Exit";
    TextComponent textComp3(str3, 90, fontPath1, Color::White, { 1920 / 2, 360 + 3 * 100 + 2 * 30 + 50 });
    textComp3.onClick = [](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            world.requestClose();
        };
    world.addComponent(exitTxt, textComp3);

    //button
    EntityID setting = world.createEntity();
    const string settingPath = "assets/Icon/Settings/B_Button44.png";
    SpriteComponent spriteComp0(settingPath, { 1620.f, 980.f }, { 5.f, 5.f });
    world.addComponent(setting, spriteComp0);

    EntityID help = world.createEntity();
    const string helpPath = "assets/Icon/Unlock/B_Button2.png";
    SpriteComponent spriteComp1(helpPath, { 1720.f, 980.f }, { 5.f, 5.f });
    world.addComponent(help, spriteComp1);

    EntityID credit = world.createEntity();
    const string creditPath = "assets/Icon/Level/B_Button53.png";
    SpriteComponent spriteComp2(creditPath, { 1820.f, 980.f }, { 5.f, 5.f });
    world.addComponent(credit, spriteComp2);

}


void MainMenu::onExit(World& world) 
{

}

void MainMenu::update(World& world, float dt) 
{

}