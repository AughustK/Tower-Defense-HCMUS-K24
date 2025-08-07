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

const int VISIBLE_COUNT = 5;
int scrollOffset = 0;

void Continue::loadSaveFiles() {
    saveFiles.clear();
    const std::string folder = "SavedGames";
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            saveFiles.push_back(entry.path().stem().string());
        }
    }
}

void Continue::spawnTextOptions(World& world) {
    const std::string fontPath = "assets/Font/Minecraft-Regular.otf";
    const std::string deletePath = "assets/Icon/Delete/B_Button68.png";

    float baseY = 300.f;
    int endIndex = std::min(scrollOffset + VISIBLE_COUNT, (int)saveFiles.size());

    for (EntityID id : textEntities)
        world.destroyEntity(id);
    textEntities.clear();

    for (int i = scrollOffset; i < endIndex; ++i) {
        const std::string& textStr = saveFiles[i];
        float y = baseY + (i - scrollOffset) * 80.f;

        // Text Entity
        EntityID textID = world.createEntity();
        registerEntity(textID);

        TextComponent text(textStr, 60, fontPath, sf::Color::White, { 960.f, y }, true, sf::Color::Black, 4.f);
        text.onClick = [textStr](EntityID eid, World& w) {
            std::cout << "[Continue] Loading: " << textStr << '\n';
            auto& sound = w.getComponent<SoundComponent>(eid);
            sound.sound->play();
            std::string savedGamePath = "SavedGames/" + textStr + ".txt";
            w.setLoadedState(std::make_unique<GamePlay>(), savedGamePath);
            };
        world.addComponent(textID, text);
        world.addComponent(textID, SoundComponent("assets/SFX/MouseClick.mp3", false));
        textEntities.push_back(textID);

        // Delete Button Entity
        EntityID delID = world.createEntity();
        registerEntity(delID);

        SpriteComponent deleteSprite(deletePath, { 1350.f, y - 20.f }, { 3.5f, 3.5f });
        deleteSprite.onClick = [this, textStr](EntityID eid, World& w) {
            std::string path = "SavedGames/" + textStr + ".txt";
            std::error_code ec;
            if (std::filesystem::remove(path, ec)) {
                std::cout << "[Continue] Deleted save file: " << path << "\n";
            }
            else {
                std::cerr << "[Continue] Failed to delete: " << ec.message() << '\n';
            }

            auto& sound = w.getComponent<SoundComponent>(eid);
            sound.sound->play();
            loadSaveFiles();              
            spawnTextOptions(w);         
            };

        world.addComponent(delID, deleteSprite);
        world.addComponent(delID, SoundComponent("assets/SFX/MouseClick.mp3", false));
        textEntities.push_back(delID);
    }
}


void Continue::spawnControlButtons(World& world) {
    const std::string upPath = "assets/Icon/Up/B_Button29.png";
    const std::string downPath = "assets/Icon/Down/B_Button20.png";
    const std::string exitPath = "assets/Icon/Left/B_Button68.png";

    // Exit Button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    SpriteComponent exitSprite(exitPath, { 100.f, 100.f }, { 5.f, 5.f });
    exitSprite.onClick = [](EntityID eid, World& w) {
        std::cout << "[Exit Button] Clicked\n";
        auto& sound = w.getComponent<SoundComponent>(eid);
        sound.sound->play();
        sf::sleep(sf::seconds(0.5f));
        w.setState(std::make_unique<MainMenu>());
        };
    SoundComponent exitSound("assets/SFX/MouseClick.mp3", false);
    world.addComponent(exitButton, exitSound);
    world.addComponent(exitButton, exitSprite);
    textEntities.push_back(exitButton);

    // Up Button
    EntityID upButton = world.createEntity();
    registerEntity(upButton);
    SpriteComponent upSprite(upPath, { 1800.f, 300.f }, { 5.f, 5.f });
    upSprite.onClick = [this](EntityID eid, World& w) {
        if (scrollOffset > 0) {
            scrollOffset--;
            spawnTextOptions(w);
        }
        auto& sound = w.getComponent<SoundComponent>(eid);
        sound.sound->play();
        };
    SoundComponent upSound("assets/SFX/MouseClick.mp3", false);
    world.addComponent(upButton, upSound);
    world.addComponent(upButton, upSprite);
    textEntities.push_back(upButton);

    // Down Button
    EntityID downButton = world.createEntity();
    registerEntity(downButton);
    SpriteComponent downSprite(downPath, { 1800.f, 700.f }, { 5.f, 5.f });
    downSprite.onClick = [this](EntityID eid, World& w) {
        if (scrollOffset + VISIBLE_COUNT < (int)saveFiles.size()) {
            scrollOffset++;
            spawnTextOptions(w);
        }
        auto& sound = w.getComponent<SoundComponent>(eid);
        sound.sound->play();
        };
    SoundComponent downSound("assets/SFX/MouseClick.mp3", false);
    world.addComponent(downButton, downSound);
    world.addComponent(downButton, downSprite);
    textEntities.push_back(downButton);
}

void Continue::onEnter(World& world) {
    std::cout << "[ContinueState] onEnter\n";
    scrollOffset = 0;
    loadSaveFiles();
    spawnTextOptions(world);
    spawnControlButtons(world);
}

void Continue::update(World&, float) {
   
}

void Continue::handleEvent(World& world, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        for (EntityID id : textEntities) {
            if (world.hasComponent<TextComponent>(id)) {
                auto& text = world.getComponent<TextComponent>(id);
                if (text.tryClick(mousePos, id, world)) return;
            }
            if (world.hasComponent<SpriteComponent>(id)) {
                auto& sprite = world.getComponent<SpriteComponent>(id);
                if (sprite.tryClick(mousePos, id, world)) return;
            }
        }
    }
}

void Continue::render(World& world, sf::RenderWindow& window) {
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);

    for (EntityID id : textEntities) {
        if (world.hasComponent<SpriteComponent>(id)) {
            auto& sprite = world.getComponent<SpriteComponent>(id);
            window.draw(sprite.sprite);
        }
    }
}
