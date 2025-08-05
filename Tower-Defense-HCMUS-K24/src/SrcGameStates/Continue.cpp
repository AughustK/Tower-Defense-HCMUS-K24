// ContinueState.cpp
#include "../../header/GameStates/Continue.h"
#include "../../header/GameStates/GamePlay.h"
#include "../../header/Components/UITextComponent.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Systems/TextRenderSystem.h"

#include <filesystem>
#include <iostream>
#include <fstream>

void Continue::loadSaveFiles() {
    saveFiles.clear();
    const std::string folder = "SavedGames";
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            saveFiles.push_back(entry.path().stem().string());  // only file name without extension
        }
    }
}

void Continue::spawnTextOptions(World& world) {
    const std::string fontPath = "assets/Font/Minecraft-Regular.otf";
    float baseY = 300.f;

    for (size_t i = 0; i < saveFiles.size(); ++i) {
        EntityID id = world.createEntity();
        registerEntity(id);

        std::string textStr = saveFiles[i];
        sf::Vector2f pos = { 960.f, baseY + i * 80.f };

        TextComponent text(textStr, 60, fontPath, sf::Color::White, pos, true, sf::Color::Black, 4.f);
        text.onClick = [textStr](EntityID eid, World& w) {
            std::cout << "[Continue] Loading: " << textStr << '\n';
            auto& sound = w.getComponent<SoundComponent>(eid);
            sound.sound->play();
            string savedGamePath = "SavedGames/" + textStr + ".txt";

            w.setLoadedState(std::make_unique<GamePlay>(), savedGamePath);
            };

        world.addComponent(id, text);
        SoundComponent sound("assets/SFX/MouseClick.mp3", false);
        world.addComponent(id, sound);
        textEntities.push_back(id);
    }
}

void Continue::onEnter(World& world) {
    std::cout << "[ContinueState] onEnter\n";
    loadSaveFiles();
    spawnTextOptions(world);
}

void Continue::update(World&, float) {
    // Not used for now
}

void Continue::handleEvent(World& world, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        for (EntityID id : textEntities) {
            auto& text = world.getComponent<TextComponent>(id);
            if (text.tryClick(mousePos, id, world)) return;
        }
    }
}

void Continue::render(World& world, sf::RenderWindow& window) {
    auto textSystem = world.getSystem<TextRenderSystem>();
    textSystem->render(world);
}
