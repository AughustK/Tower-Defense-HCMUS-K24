#include "../../header/GameStates/Continue.h"
#include "../../header/GameStates/GamePlay.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/Components/UITextComponent.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/UISpriteComponent.h"
#include "../../header/Systems/TextRenderSystem.h"

#include <filesystem>
#include <iostream>
#include <fstream>

void Continue::loadSaveFiles() {
    saveFiles.clear();
    const std::string folder = "SavedGames";
    if (!std::filesystem::exists(folder)) return;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            saveFiles.push_back(entry.path().stem().string());
        }
    }
}

void Continue::clampScroll() {
    int total = (int)saveFiles.size();
    if (total <= VISIBLE_COUNT) {
        scrollOffset = 0;
        return;
    }
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > total - VISIBLE_COUNT) scrollOffset = total - VISIBLE_COUNT;
}

void Continue::spawnTextOptions(World& world) {
    for (EntityID id : fileTextEntities) {
        world.destroyEntity(id);
    }
    fileTextEntities.clear();

    for (EntityID id : fileDeleteEntities) {
        world.destroyEntity(id);
    }
    fileDeleteEntities.clear();

    const std::string fontPath = "assets/Font/Minecraft-Regular.otf";
    const std::string deletePath = "assets/Icon/Close/C_Buttons41.png";

    float baseY = 300.f;
    float textX = 760.f;
    float deleteX = 1350.f;
    float lineSpacing = 80.f;

    for (int slot = 0; slot < VISIBLE_COUNT; ++slot) {
        int idx = scrollOffset + slot;
        float y = baseY + slot * lineSpacing;

        EntityID textID = world.createEntity();
        registerEntity(textID);

        std::string label = "";
        bool active = false;
        if (idx < (int)saveFiles.size()) {
            label = saveFiles[idx];
            active = true;
        }

        TextComponent textComp(label, 60, fontPath, sf::Color::White, { textX, y }, true, sf::Color::Black, 4.f);

        if (active) {
            textComp.onClick = [idx, label](EntityID eid, World& w) {
                std::cout << "[Continue] Loading: " << label << '\n';
                if (w.hasComponent<SoundComponent>(eid)) {
                    auto& sound = w.getComponent<SoundComponent>(eid);
                    if (sound.sound) sound.sound->play();
                }
                std::string savedGamePath = "SavedGames/" + label + ".txt";
                w.setLoadedState(std::make_unique<GamePlay>(), savedGamePath);
                };
        }

        world.addComponent(textID, textComp);
        if (active) world.addComponent(textID, SoundComponent("assets/SFX/MouseClick.mp3", false));

        fileTextEntities.push_back(textID);

        if (active) {
            EntityID delID = world.createEntity();
            registerEntity(delID);

            SpriteComponent deleteSprite(deletePath, { deleteX, y - 20.f }, { 3.5f, 3.5f });
            deleteSprite.onClick = [this, idx](EntityID eid, World& w) {
                if (idx >= 0 && idx < (int)saveFiles.size()) {
                    std::string textStr = saveFiles[idx];
                    std::string srcPath = "SavedGames/" + textStr + ".txt";
                    std::string trashDir = "Trash";
                    std::string dstPath = trashDir + "/" + textStr + ".txt";

                    if (!std::filesystem::exists(trashDir)) {
                        std::filesystem::create_directory(trashDir);
                    }

                    std::error_code ec;
                    std::filesystem::rename(srcPath, dstPath, ec);

                    if (!ec) {
                        std::cout << "[Continue] Moved save file to Trash: " << dstPath << "\n";
                    }
                    else {
                        std::cerr << "[Continue] Failed to move: " << ec.message() << "\n";
                    }

                    if (w.hasComponent<SoundComponent>(eid)) {
                        auto& sound = w.getComponent<SoundComponent>(eid);
                        if (sound.sound) sound.sound->play();
                    }

                    loadSaveFiles();
                    clampScroll();
                    spawnTextOptions(w);
                }
                };
            world.addComponent(delID, deleteSprite);
            world.addComponent(delID, SoundComponent("assets/SFX/MouseClick.mp3", false));
            fileDeleteEntities.push_back(delID);
        }
    }
}

void Continue::spawnControlButtons(World& world) {
    if (!controlEntities.empty()) return;

    const std::string upPath = "assets/Icon/Up/B_Button29.png";
    const std::string downPath = "assets/Icon/Down/B_Button20.png";
    const std::string exitPath = "assets/Icon/Left/B_Button68.png";

    // Exit Button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    SpriteComponent exitSprite(exitPath, { 0.f, 0.f }, { 5.f, 5.f });
    exitSprite.onClick = [](EntityID eid, World& w) {
        std::cout << "[Exit Button] Clicked\n";
        if (w.hasComponent<SoundComponent>(eid)) {
            auto& sound = w.getComponent<SoundComponent>(eid);
            if (sound.sound) sound.sound->play();
        }
        sf::sleep(sf::seconds(0.5f));
        w.setState(std::make_unique<MainMenu>());
        };
    world.addComponent(exitButton, SoundComponent("assets/SFX/MouseClick.mp3", false));
    world.addComponent(exitButton, exitSprite);
    controlEntities.push_back(exitButton);

    // Up Button
    EntityID upButton = world.createEntity();
    registerEntity(upButton);
    SpriteComponent upSprite(upPath, { 1800.f, 180.f }, { 5.f, 5.f });
    upSprite.onClick = [this](EntityID eid, World& w) {
        if (scrollOffset > 0) {
            --scrollOffset;
            spawnTextOptions(w);
        }
        if (w.hasComponent<SoundComponent>(eid)) {
            auto& sound = w.getComponent<SoundComponent>(eid);
            if (sound.sound) sound.sound->play();
        }
        };
    world.addComponent(upButton, SoundComponent("assets/SFX/MouseClick.mp3", false));
    world.addComponent(upButton, upSprite);
    controlEntities.push_back(upButton);

    // Down Button
    EntityID downButton = world.createEntity();
    registerEntity(downButton);
    SpriteComponent downSprite(downPath, { 1800.f, 700.f }, { 5.f, 5.f });
    downSprite.onClick = [this](EntityID eid, World& w) {
        if (scrollOffset + VISIBLE_COUNT < (int)saveFiles.size()) {
            ++scrollOffset;
            spawnTextOptions(w);
        }
        if (w.hasComponent<SoundComponent>(eid)) {
            auto& sound = w.getComponent<SoundComponent>(eid);
            if (sound.sound) sound.sound->play();
        }
        };
    world.addComponent(downButton, SoundComponent("assets/SFX/MouseClick.mp3", false));
    world.addComponent(downButton, downSprite);
    controlEntities.push_back(downButton);
}

void Continue::onEnter(World& world) {
    std::cout << "[ContinueState] onEnter\n";
    scrollOffset = 0;
    loadSaveFiles();
    clampScroll();

    spawnControlButtons(world); 
    spawnTextOptions(world);    
}

void Continue::update(World&, float) {

}

void Continue::handleEvent(World& world, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        for (EntityID id : fileTextEntities) {
            if (world.hasComponent<TextComponent>(id)) {
                auto& text = world.getComponent<TextComponent>(id);
                if (text.tryClick(mousePos, id, world)) return;
            }
        }

        for (EntityID id : fileDeleteEntities) {
            if (world.hasComponent<SpriteComponent>(id)) {
                auto& sprite = world.getComponent<SpriteComponent>(id);
                if (sprite.tryClick(mousePos, id, world)) return;
            }
        }

        for (EntityID id : controlEntities) {
            if (world.hasComponent<SpriteComponent>(id)) {
                auto& sprite = world.getComponent<SpriteComponent>(id);
                if (sprite.tryClick(mousePos, id, world)) return;
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseMove.x, event.mouseMove.y });

        for (EntityID id : fileTextEntities) {
            if (world.hasComponent<TextComponent>(id)) {
                auto& text = world.getComponent<TextComponent>(id);
                text.tryHover(mousePos, id, world);
            }
        }
    }
}

void Continue::render(World& world, sf::RenderWindow& window) {
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);

    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}