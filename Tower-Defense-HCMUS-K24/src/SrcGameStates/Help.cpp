#include "../../header/GameStates/Help.h"
#include "../../header/GameStates/MainMenu.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"

#include "../../header/Components/SoundComponent.h"

void Help::onEnter(World& world)
{
    std::cout << "[Help] onEnter called\n";

    const string soundPath = "assets/SFX/MouseClick.mp3";
    const vector<string> slidePaths = {
            "assets/HelpSlides/slide1.png",
            "assets/HelpSlides/slide2.png",
            "assets/HelpSlides/slide3.png",
            "assets/HelpSlides/slide4.png",
            "assets/HelpSlides/slide5.png"
    };

    currentSlide = 0;
    slides = slidePaths;

    SoundComponent soundComp(soundPath, false);
    soundComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);

    float volume = world.getSystem<SoundSystem>()->globalVolume;

    //slide
    EntityID slide = world.createEntity();
    registerEntity(slide);
    this->slideBoxEntity = slide;
    SpriteComponent slideSprite(slidePaths[currentSlide], { 0.f, 0.f }, { 1.f, 1.f });
    world.addComponent(slide, slideSprite);

    //button

    //left button
    EntityID leftButton = world.createEntity();
    registerEntity(leftButton);
    const string leftButtonPath = "assets/Icon/Back/B_Button56.png";
    SpriteComponent spriteLeftButton(leftButtonPath, { 0.f, 995.f }, { 4.f, 4.f });
    spriteLeftButton.onClick = [this](EntityID entityID, World& world)
        {
            std::cout << "[Left Button] Clicked\n";
            if (world.hasComponent<SoundComponent>(entityID))
            {
                auto& sound = world.getComponent<SoundComponent>(entityID);
                sound.sound->play();
            }
            if (currentSlide > 0) currentSlide--;
            else shouldExit = true;
            updateSlide(world);
        };
    world.addComponent(leftButton, spriteLeftButton);
    world.addComponent(leftButton, SoundComponent(soundPath, false, volume));

    //right button
    EntityID rightButton = world.createEntity();
    registerEntity(rightButton);
    const string rightButtonPath = "assets/Icon/Next/B_Button59.png";
    SpriteComponent spriteRightButton(rightButtonPath, { 85.f, 995.f }, { 4.f, 4.f });
    spriteRightButton.onClick = [this](EntityID entityID, World& world)
        {
            std::cout << "[Right Button] Clicked\n";
            if (world.hasComponent<SoundComponent>(entityID))
            {
                auto& sound = world.getComponent<SoundComponent>(entityID);
                sound.sound->play();
            }
            if (currentSlide < slides.size() - 1) currentSlide++;
            else shouldExit = true;
            updateSlide(world);
        };
    world.addComponent(rightButton, spriteRightButton);
    world.addComponent(rightButton, SoundComponent(soundPath, false, volume));
}

void Help::updateSlide(World& world)
{
    auto& spriteComp = world.getComponent<SpriteComponent>(slideBoxEntity);
    if (!spriteComp.texture->loadFromFile(slides[currentSlide])) {
        std::cerr << "[Help] Failed to load slide: " << slides[currentSlide] << "\n";
        return;
    }
    spriteComp.sprite.setTexture(*spriteComp.texture, true);
}

void Help::render(World& world, sf::RenderWindow& window)
{
    auto spriteSystem = world.getSystem<SpriteRenderSystem>();
    spriteSystem->render(world);
}

void Help::handleEvent(World& world, sf::Event& event)
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
            sf::FloatRect bounds = spriteComp.sprite.getGlobalBounds();
            std::cout << "[Help] Entity " << e << " bounds: ("
                << bounds.left << ", " << bounds.top << ", "
                << bounds.width << ", " << bounds.height << ")\n";

            if (spriteComp.tryClick(mousePos, e, world))
            {
                std::cout << "[Help] Successfully clicked entity " << e << "\n";
                return;
            }
        }
    }
}

void Help::update(World& world, float dt)
{
    if (shouldExit)
    {
        shouldExit = false;
        world.setState(std::make_unique<MainMenu>());
        return;
    }
    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) musicSystem->play(world);
}

void Help::onExit(World& world)
{
    GameState::onExit(world);
    std::cout << "[Help] onExit called\n";
}