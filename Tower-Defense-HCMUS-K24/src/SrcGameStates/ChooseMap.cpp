#include "../../header/Managers/World.h"
#include "../../header/GameStates/ChooseMap.h"
#include "../../header/GameStates/Lobby.h"

#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"
#include "../../header/Components/UITextComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/TextRenderSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Systems/InitializeEnemy.h"
#include "../../header/Systems/InitializeProjectile.h"



void ChooseMap::onEnter(World& world)
{
    std::cout << "[ChooseMap] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    const std::string font = "assets/Font/Minecraft-Regular.otf";
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
    SpriteComponent spriteComp2(blackFlagPath, { 1300.f, 190.f }, { 0.5f, 0.5f }, true);
    spriteComp2.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][HellMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));

            pendingMap = "HellMap";
            showDifficultyMenu(world);
        };
    world.addComponent(blackFlag, soundComp);
    world.addComponent(blackFlag, spriteComp2);
    auto& sp1 = world.getComponent<SpriteComponent>(blackFlag);
    sf::FloatRect b1 = sp1.sprite.getLocalBounds();
    sp1.sprite.setOrigin(b1.width / 2.f, b1.height / 2.f);

    //ice map
    EntityID blueFlag = world.createEntity();
    registerEntity(blueFlag);
    const string blueFlagPath = "assets/Bg/BlueFlag.png";
    SpriteComponent spriteComp3(blueFlagPath, { 1530.f, 510.f }, { 0.5f, 0.5f }, true);
    spriteComp3.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][IceMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            pendingMap = "IceMap";
            showDifficultyMenu(world);
        };
    world.addComponent(blueFlag, soundComp);
    world.addComponent(blueFlag, spriteComp3);
    auto& sp2 = world.getComponent<SpriteComponent>(blueFlag);
    sf::FloatRect b2 = sp2.sprite.getLocalBounds();
    sp2.sprite.setOrigin(b2.width / 2.f, b2.height / 2.f);

    //paradise map
    EntityID whiteFlag = world.createEntity();
    registerEntity(whiteFlag);
    const string whiteFlagPath = "assets/Bg/WhiteFlag.png";
    SpriteComponent spriteComp4(whiteFlagPath, { 1270.f, 820.f }, { 0.5f, 0.5f }, true);
    spriteComp4.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][ParadiselMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            pendingMap = "ParadiseMap";
            showDifficultyMenu(world);
        };
    world.addComponent(whiteFlag, soundComp);
    world.addComponent(whiteFlag, spriteComp4);
    auto& sp3 = world.getComponent<SpriteComponent>(whiteFlag);
    sf::FloatRect b3 = sp3.sprite.getLocalBounds();
    sp3.sprite.setOrigin(b3.width / 2.f, b3.height / 2.f);

    //firemap
    EntityID redFlag = world.createEntity();
    registerEntity(redFlag);
    const string redFlagPath = "assets/Bg/redFlag.png";
    SpriteComponent spriteComp5(redFlagPath, { 620.f, 410.f }, { 0.5f, 0.5f }, true);
    spriteComp5.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[ChooseMap][FireMap] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityId);
            sound.sound->play();
            sf::sleep(sf::seconds(0.5f));
            pendingMap = "FireMap";
            showDifficultyMenu(world);
        };
    world.addComponent(redFlag, soundComp);
    world.addComponent(redFlag, spriteComp5);
    auto& sp4 = world.getComponent<SpriteComponent>(redFlag);
    sf::FloatRect b4 = sp4.sprite.getLocalBounds();
    sp4.sprite.setOrigin(b4.width / 2.f, b4.height / 2.f);

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

    //map name
    // HellMap text
    EntityID hellText = world.createEntity();
    registerEntity(hellText);
    {
        auto& sp = world.getComponent<SpriteComponent>(blackFlag).sprite;
        sf::Vector2f pos = { sp.getPosition().x-30, sp.getPosition().y + sp.getGlobalBounds().height / 2.f + 30.f };
        TextComponent text("Infernal Depths", 30, font, sf::Color(128, 0, 128), pos, false, sf::Color(255, 215, 0), 2.f);
        world.addComponent(hellText, text);
    }

    // IceMap text
    EntityID iceText = world.createEntity();
    registerEntity(iceText);
    {
        auto& sp = world.getComponent<SpriteComponent>(blueFlag).sprite;
        sf::Vector2f pos = { sp.getPosition().x -30, sp.getPosition().y + sp.getGlobalBounds().height / 2.f + 30.f };
        TextComponent text("Frozen Expanse", 35, font, sf::Color(173, 216, 230), pos, false, sf::Color(0, 0, 128), 2.f);
        world.addComponent(iceText, text);
    }

    // ParadiseMap text
    EntityID paradiseText = world.createEntity();
    registerEntity(paradiseText);
    {
        auto& sp = world.getComponent<SpriteComponent>(whiteFlag).sprite;
        sf::Vector2f pos = { sp.getPosition().x-30, sp.getPosition().y + sp.getGlobalBounds().height / 2.f + 30.f };
        TextComponent text("Celestial Haven", 35, font, sf::Color(255, 253, 208), pos, false, sf::Color(0, 128, 128), 2.f);
        world.addComponent(paradiseText, text);
    }

    // FireMap text
    EntityID fireText = world.createEntity();
    registerEntity(fireText);
    {
        auto& sp = world.getComponent<SpriteComponent>(redFlag).sprite;
        sf::Vector2f pos = { sp.getPosition().x-30, sp.getPosition().y + sp.getGlobalBounds().height / 2.f + 30.f };
        TextComponent text("Blazing Inferno", 35, font, sf::Color::Red, pos, false, sf::Color(255, 215, 0), 2.f);
        world.addComponent(fireText, text);
    }

}

void ChooseMap::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);

    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);
}

void ChooseMap::handleEvent(World& world, sf::Event& event)
{
    sf::Vector2f mousePos;
    if (event.type == sf::Event::MouseMoved) {
        mousePos = world.window.mapPixelToCoords(
            { event.mouseMove.x, event.mouseMove.y });
    } 
    else if (event.type == sf::Event::MouseButtonPressed) {
        mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });
    }

    if (showingDifficultyMenu)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            for (EntityID btn : difficultyButtons)
            {
                if (world.hasComponent<TextComponent>(btn)) 
                {
                    auto& tc = world.getComponent<TextComponent>(btn);
                    tc.tryHover(mousePos, btn, world);
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            bool clickedDifficulty = false;
            for (EntityID btn : difficultyButtons)
            {
                if (world.hasComponent<TextComponent>(btn)) {
                    auto& tc = world.getComponent<TextComponent>(btn);
                    if (tc.tryClick(mousePos, btn, world))
                    {
                        clickedDifficulty = true;
                        break;
                    }
                }
            }
            if (!clickedDifficulty)
            {
                for (EntityID btn : difficultyButtons)
                    world.destroyEntity(btn);
                difficultyButtons.clear();
                showingDifficultyMenu = false;
                pendingMap.reset();
            }
        }
        return; 
    }

    if (event.type == sf::Event::MouseMoved)
    {
        for (EntityID e : world.getEntitiesWithComponent<TextComponent>())
        {
            auto& tc = world.getComponent<TextComponent>(e);
            tc.tryHover(mousePos, e, world);
        }
        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
        {
            auto& sp = world.getComponent<SpriteComponent>(e);

            if (!sp.isHover) continue;

            sf::FloatRect bounds = sp.sprite.getGlobalBounds();
            if (bounds.contains(mousePos))
            {
                sp.sprite.setScale(0.6f, 0.6f);
            }
            else
            {
                sp.sprite.setScale(0.5f, 0.5f);
            }
        }
    }

    else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        // click TextComponent
        for (EntityID e : world.getEntitiesWithComponent<TextComponent>())
        {
            auto& tc = world.getComponent<TextComponent>(e);
            if (tc.tryClick(mousePos, e, world))
                return;
        }
        // click SpriteComponent
        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
        {
            auto& sp = world.getComponent<SpriteComponent>(e);
            if (sp.tryClick(mousePos, e, world))
                return;
        }
        // click ClickComponent
        for (EntityID e : world.getEntitiesWithComponent<ClickComponent>())
        {
            auto& cc = world.getComponent<ClickComponent>(e);
            if (cc.tryClick(mousePos, e, world))
                return;
        }
    }
}

void ChooseMap::update(World& world, float dt)
{
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}


void ChooseMap::showDifficultyMenu(World& world)
{
    showingDifficultyMenu = true;
    difficultyButtons.clear();

    float centerX = 960.f;
    float centerY = 540.f;

    const string fontPath = "assets/Font/Minecraft-Regular.otf";
    std::vector<std::pair<std::string, DifficultyLevel>> options = {
        {"EASY",   DifficultyLevel::Easy},
        {"NORMAL", DifficultyLevel::Normal},
        {"HARD",   DifficultyLevel::Hard}
    };

    float spacingY = 100.f;
    float totalSpan = spacingY * (options.size() - 1); // 200.f
    float startY = centerY - totalSpan * 0.5f;         // 440.f

    vector<sf::Color> btnColor = { Color::Green, Color::Yellow, Color::Red };

    EntityID bg = world.createEntity();
    const std::string bgPath = "assets/Bg/FrameLevel.png";
    SpriteComponent bgSprite(bgPath, { centerX, centerY + 40.f }, { 1.f, 1.f });
    sf::FloatRect bounds = bgSprite.sprite.getLocalBounds();
    bgSprite.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    world.addComponent(bg, bgSprite);
    difficultyButtons.push_back(bg);

    for (size_t i = 0; i < options.size(); ++i) {
        EntityID btn = world.createEntity();

        TextComponent tc(
            options[i].first,
            48,
            fontPath,
            Color::White,
            { centerX, startY + spacingY * float(i) },
            true,
            sf::Color::Black,
            5.f,
            btnColor[i]
        );

        tc.onClick = [this, lvl = options[i].second](EntityID, World& w)
            {
                this->selectedDifficulty = lvl;

                for (EntityID d : difficultyButtons)
                    w.destroyEntity(d);
                difficultyButtons.clear();
                showingDifficultyMenu = false;

                if (pendingMap.has_value()) {
                    w.setState(
                        std::make_unique<GamePlay>(
                            pendingMap.value(),
                            this->selectedDifficulty
                        )
                    );
                }
            };

        world.addComponent(btn, tc);
        difficultyButtons.push_back(btn);
    }
}



