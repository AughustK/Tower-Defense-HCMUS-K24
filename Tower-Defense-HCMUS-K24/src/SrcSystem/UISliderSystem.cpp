#pragma once
#include <SFML/Graphics.hpp>
#include "../../header/Systems/System.h"
#include "../../header/Managers/World.h"
#include "../../header/Components/UISliderComponent.h"
#include "../../header/Systems/SoundSystem.h"
#include "../../header/Systems/MusicSystem.h"
#include "../../header/Systems/UISliderSystem.h"

void SliderSystem::render(World& world)
{
    for (EntityID entity : entities)
    {
        auto& sliderComp = world.getComponent<SliderComponent>(entity);
        world.window.draw(sliderComp.track);
        world.window.draw(sliderComp.fill);
        world.window.draw(sliderComp.handle);
        world.window.draw(sliderComp.label);
        world.window.draw(sliderComp.valueText);
    }
}

void SliderSystem::update(float deltaTime)
{

}

void SliderSystem::handleEvent(World& world) 
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(world.window);
    sf::Vector2f worldMousePos = world.window.mapPixelToCoords(mousePos);

    for (EntityID entity : entities)
    {
        auto& slider = world.getComponent<SliderComponent>(entity);

        sf::FloatRect handleBounds = slider.handle.getGlobalBounds();
        bool isMouseOverHandle = handleBounds.contains(worldMousePos);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!slider.isDragging && isMouseOverHandle)
            {
                slider.isDragging = true;
            }

            if (slider.isDragging)
            {
                float x = slider.track.getPosition().x;
                float width = slider.track.getSize().x;

                float clampedX = std::clamp(worldMousePos.x, x, x + width);

                float percent = (clampedX - x) / width;
                slider.value = slider.min + percent * (slider.max - slider.min);

                slider.updateHandlePosition();

                if (slider.type == "SFX:")
                {
                    auto soundSystem = world.getSystem<SoundSystem>();
                    soundSystem->globalVolume = slider.value;
                }
                else if (slider.type == "Music:")
                {
                    auto musicSystem = world.getSystem<MusicSystem>();
                    musicSystem->globalVolume=slider.value;
                }
            }
        }
        else
        {
            slider.isDragging = false;
        }
    }
}