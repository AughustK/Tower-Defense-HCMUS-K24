#include "../../header/Managers/World.h"
#include "../../header/GameStates/MainMenu.h"
#include "../../header/GameStates/Lobby.h"
#include "../../header/GameStates/Story.h"
#include "../../header/GameStates/ChooseMap.h"

#include "../../header/Components/SoundComponent.h"
#include "../../header/Components/MusicComponent.h"
#include "../../header/Components/ClickComponent.h"

#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"


void Story::onEnter(World& world)
{
    std::cout << "[Story] onEnter called\n";

    reachedLastSlide = false;

    auto musicSystem = world.getSystem<MusicSystem>();
    if (musicSystem) {
        musicSystem->play(world);
        musicSystem->pauseAll(world);
    }

    const std::string soundPath = "assets/SFX/MouseClick.mp3";
    float volume = world.getSystem<SoundSystem>()->globalVolume;

    slides = {
        "assets/StorySlides/slide1.png", "assets/StorySlides/slide2.png",
        "assets/StorySlides/slide3.png", "assets/StorySlides/slide4.png",
        "assets/StorySlides/slide5.png", "assets/StorySlides/slide6.png",
        "assets/StorySlides/slide7.png", "assets/StorySlides/slide8.png"
    };

    voice = {
        "assets/SFX/StoryVoice/1.mp3", "assets/SFX/StoryVoice/2.mp3",
        "assets/SFX/StoryVoice/3.mp3", "assets/SFX/StoryVoice/4.mp3",
        "assets/SFX/StoryVoice/5.mp3", "assets/SFX/StoryVoice/6.mp3",
        "assets/SFX/StoryVoice/7.mp3", "assets/SFX/StoryVoice/8.mp3"
    };

    currentSlide = 0;

    // Voice entity
    voiceEntityID = world.createEntity();
    registerEntity(voiceEntityID);

    // Slide entity
    slideBoxEntity = world.createEntity();
    registerEntity(slideBoxEntity);
    world.addComponent(slideBoxEntity, SpriteComponent(slides[0], { 0.f, 0.f }, { 1.f, 1.f }));

    // Skip button
    EntityID skip = world.createEntity();
    registerEntity(skip);
    TextComponent skipComp("...Skip", 60, "assets/Font/computer-says-no.otf",
        Color::White, { 1840.f, 0.f }, true, Color::Black, 2.f);
    skipComp.onClick = [this](EntityID id, World& world) {
        std::cout << "[Skip Text] Clicked\n";
        if (world.hasComponent<SoundComponent>(id))
            world.getComponent<SoundComponent>(id).sound->play();
        sf::sleep(sf::seconds(0.3f));
        currentSlide = static_cast<int>(slides.size()) - 1;
        updateSlide(world);
        };
    world.addComponent(skip, skipComp);
    world.addComponent(skip, SoundComponent(soundPath, false, volume));

    updateSlide(world);
}

void Story::updateSlide(World& world)
{
    // Update background slide
    auto& sprite = world.getComponent<SpriteComponent>(slideBoxEntity);
    if (!sprite.texture->loadFromFile(slides[currentSlide])) {
        std::cerr << "[Story] Failed to load slide: " << slides[currentSlide] << "\n";
        return;
    }
    sprite.sprite.setTexture(*sprite.texture, true);

    // Update voice
    if (world.hasComponent<SoundComponent>(voiceEntityID)) {
        world.removeComponent<SoundComponent>(voiceEntityID);
    }
    SoundComponent voiceComp(voice[currentSlide], false);
    voiceComp.sound->setVolume(world.getSystem<SoundSystem>()->globalVolume);
    world.addComponent(voiceEntityID, voiceComp);
    voiceComp.sound->play();

    // Check if last slide
    if (currentSlide >= slides.size() - 1) {
        for (EntityID e : world.getEntitiesWithComponent<TextComponent>()) {
            auto& text = world.getComponent<TextComponent>(e);
            if (text.txt.getString() == "...Skip") 
            {
                world.removeComponent<TextComponent>(e);
            }
        }
        reachedLastSlide = true;
        lastSlideClock.restart();
    }
}

void Story::render(World& world, sf::RenderWindow& window)
{
    world.getSystem<SpriteRenderSystem>()->render(world);
    for (EntityID e : world.getEntitiesWithComponent<TextComponent>()) {
        auto& tc = world.getComponent<TextComponent>(e);
        window.draw(tc.txt);
    }
}

void Story::handleEvent(World& world, sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mouse = world.window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
        for (EntityID e : world.getEntitiesWithComponent<SpriteComponent>())
            if (world.getComponent<SpriteComponent>(e).tryClick(mouse, e, world))
                return;
        for (EntityID e : world.getEntitiesWithComponent<TextComponent>())
            if (world.getComponent<TextComponent>(e).tryClick(mouse, e, world))
                return;

        if (currentSlide < slides.size() - 1) {
            currentSlide++;
            updateSlide(world);
            return;
        }
    }
}

void Story::update(World& world, float dt)
{
    if (shouldExit) {
        shouldExit = false;
        world.setState(std::make_unique<Lobby>());
    }
    if (reachedLastSlide && lastSlideClock.getElapsedTime().asSeconds() >= 9.0f) {
        std::cout << "[Story] Auto-exit after finished last slide\n";
        shouldExit = true;
    }
}

void Story::onExit(World& world)
{
    GameState::onExit(world);
    if (auto musicSystem = world.getSystem<MusicSystem>())
        musicSystem->resumeAll(world);
    reachedLastSlide = false;
    std::cout << "[Story] onExit called\n";
}
