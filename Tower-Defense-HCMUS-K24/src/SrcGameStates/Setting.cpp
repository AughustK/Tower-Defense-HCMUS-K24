#include "../../header/GameStates/Setting.h"
#include "../../header/GameStates/MainMenu.h"

#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/UISliderComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/UISliderSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Managers/World.h"



void Setting::handleEvent(World& world, sf::Event& event) {
    auto entities = world.getEntitiesWithComponent<SpriteComponent>();
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        Vector2f mousePos = world.window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });
        for (EntityID e : entities)
        {
            std::cout << "[Setting][Handle Event]\n";
            std::cout << "[Entity] ID = " << e << "\n";
            std::cout << "  - Has SpriteComponent: " << world.hasComponent<SpriteComponent>(e) << "\n";
            std::cout << "  - Has TextComponent: " << world.hasComponent<TextComponent>(e) << "\n";
            std::cout << "  - Has SoundComponent: " << world.hasComponent<SoundComponent>(e) << "\n";
            auto& spriteComp = world.getComponent<SpriteComponent>(e);
            spriteComp.tryClick(mousePos, e, world);
        }
    }
    world.getSystem<SliderSystem>()->handleEvent(world);
}

void Setting::render(World& world, sf::RenderWindow& window) {
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
    auto sliderSystem = world.getSystem<SliderSystem>();
    sliderSystem->render(world);
}

void Setting::onEnter(World& world) {
    std::cout << "[Setting] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    SoundComponent soundComp(soundPath, false);


    auto winSize = world.window.getSize();
    float centerX = static_cast<float>(winSize.x) / 2.f;


    float sliderWidth = 600.f;
    float sliderX = centerX - sliderWidth / 2.f;

    //slider
    EntityID soundSlider = world.createEntity();
    registerEntity(soundSlider);
    float soundVolume = world.getSystem<SoundSystem>()->globalVolume;
    SliderComponent sliderComp0(sliderWidth, sliderX, 480.f, "SFX:", soundVolume);
    world.addComponent(soundSlider, sliderComp0);

    EntityID musicSlider = world.createEntity();
    registerEntity(musicSlider);
    float musicVolume = world.getSystem<MusicSystem>()->globalVolume;
    SliderComponent sliderComp1(sliderWidth, sliderX, 600.f, "Music:", musicVolume);
    world.addComponent(musicSlider, sliderComp1);

    //bg
    EntityID background = world.createEntity();
    registerEntity(background);
    const string bgPath = "assets/Bg/SettingBg.png";
    SpriteComponent spriteComp0(bgPath, { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(background, spriteComp0);

    //button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    const string buttonPath = "assets/Icon/Left/B_Button68.png";
    SpriteComponent spriteComp1(buttonPath, { 0.f, 0.f }, { 5.f, 5.f });
    spriteComp1.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            if (world.hasComponent<SoundComponent>(entityId)) {
                auto& sound = world.getComponent<SoundComponent>(entityId);
                if (sound.sound)
                {
                    sound.sound->play();
                    sf::sleep(sf::seconds(0.5f));
                }
            }
            shouldExit = true;
        };
    world.addComponent(exitButton, spriteComp1);
	world.addComponent(exitButton, soundComp);
}

void Setting::update(World& world, float dt) {
    if (shouldExit) {
        shouldExit = false;
        world.setState(std::make_unique<MainMenu>());
    }
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);

    auto soundSystem = world.getSystem<SoundSystem>();
    if (soundSystem) soundSystem->play(world);
}