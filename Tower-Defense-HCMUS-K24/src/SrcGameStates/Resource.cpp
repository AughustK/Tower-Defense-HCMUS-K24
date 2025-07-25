#include "../../header/Managers/World.h"
#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/GameStates/Lobby.h"
#include "../../header/GameStates/Resource.h"
#include "../../header/GameStates/ChooseMap.h"


void Resource::onEnter(World& world)
{
    std::cout << "[Resource] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const string musicPath = "assets/SFX/Music/Main/Main_Final.mp3";
    const vector<string> slidePaths = {
            "assets/ResourceSlides/slide1.png",
            "assets/ResourceSlides/slide2.png",
            "assets/ResourceSlides/slide3.png",
            "assets/ResourceSlides/slide4.png",
            "assets/ResourceSlides/slide5.png",
            "assets/ResourceSlides/slide6.png"
    };

    currentSlide = 0;
    slides = slidePaths;

    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    float volume = world.getSystem<SoundSystem>()->globalVolume;
    MusicComponent musicComp(musicPath);

    //slide
    EntityID slide = world.createEntity();
    registerEntity(slide);
    this->slideBoxEntity = slide;
    SpriteComponent slideSprite(slidePaths[currentSlide], { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(slide, slideSprite);

    //button
        //exit button
    EntityID exitButton = world.createEntity();
    registerEntity(exitButton);
    const string buttonPath = "assets/Icon/Left/B_Button68.png";
    SpriteComponent spriteComp1(buttonPath, { 0.f, 0.f }, { 5.f, 5.f });
    spriteComp1.onClick = [this](EntityID entityId, World& world)
        {
            std::cout << "[Exit Button] Clicked\n";
            if (world.hasComponent<SoundComponent>(entityId))
            {
                auto& sound = world.getComponent<SoundComponent>(entityId);
                sound.sound->play();
                sf::sleep(sf::seconds(0.5f));
            }
            shouldExit = true;
        };
    world.addComponent(exitButton, soundComp);
    world.addComponent(exitButton, spriteComp1);

    //left button
    EntityID leftButton = world.createEntity();
    registerEntity(leftButton);
    const string leftButtonPath = "assets/Icon/Back/B_Button56.png";
    SpriteComponent spriteLeftButton(leftButtonPath, { 850.f, 895.f }, { 4.f, 4.f });
    spriteLeftButton.onClick = [this](EntityID entityID, World& world)
        {
            std::cout << "[Left Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityID);
            sound.sound->play();
            if (currentSlide <= 0) currentSlide = slides.size() - 1;
            else
            {
                currentSlide--;
            }
            updateSlide(world);
        };
    world.addComponent(leftButton, spriteLeftButton);
    world.addComponent(leftButton, SoundComponent(soundPath, false, volume));

    //right button
    EntityID rightButton = world.createEntity();
    registerEntity(rightButton);
    const string rightButtonPath = "assets/Icon/Next/B_Button59.png";
    SpriteComponent spriteRightButton(rightButtonPath, { 970.f, 895.f }, { 4.f, 4.f });
    spriteRightButton.onClick = [this](EntityID entityID, World& world)
        {
            std::cout << "[Right Button] Clicked\n";
            auto& sound = world.getComponent<SoundComponent>(entityID);
            sound.sound->play();
            if (currentSlide >= slides.size() - 1) currentSlide = 0;
            else
            {
                currentSlide++;
            }
            updateSlide(world);
        };
    world.addComponent(rightButton, spriteRightButton);
    world.addComponent(rightButton, SoundComponent(soundPath, false, volume));
}

void Resource::updateSlide(World& world)
{
    auto& spriteComp = world.getComponent<SpriteComponent>(slideBoxEntity);
    if (!spriteComp.texture->loadFromFile(slides[currentSlide])) {
        std::cerr << "[Resource] Failed to load slide: " << slides[currentSlide] << "\n";
        return;
    }
    spriteComp.sprite.setTexture(*spriteComp.texture, true);
}

void Resource::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void Resource::handleEvent(World& world, sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
        {
            auto& sc = world.getComponent<SpriteComponent>(e);
            if (sc.tryClick(mousePos, e, world))
                return;
        }
    }
}

void Resource::update(World& world, float dt)
{
    if (shouldExit)
    {
        shouldExit = false;
        world.setState(std::make_unique<Lobby>());
        return;
    }
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}

void Resource::onExit(World& world)
{
    GameState::onExit(world);
    std::cout << "[Resource] onExit called\n";
}