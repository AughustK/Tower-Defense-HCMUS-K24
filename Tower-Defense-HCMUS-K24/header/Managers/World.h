#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include <SFML/Graphics.hpp>
#include "../System.h"  
#include "GameState.h"
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

class System;                        

class World
{
private:
    EntityManager    entityManager;       
    ComponentManager componentManager;    
    SystemManager systemManager;

    std::unique_ptr<GameState> currentState=nullptr;
    bool shouldClose = false;

public:
    sf::RenderWindow window;
    World();
    ~World() = default;

    void init();

    // Entity creation/destruction
    EntityID createEntity();
    void destroyEntity(EntityID entityID);


    // Component register, management
    template <typename T>
    void registerComponent()
    {
        componentManager.registerComponent<T>();
    }

    template<typename T>
    void addComponent(EntityID entityID, const T& component)
    {
        componentManager.addComponent<T>(entityID, component);

        // Update entity signature
        ComponentID compID = componentManager.getComponentType<T>();
        Signature signature = entityManager.getSignature(entityID);
        signature.set(compID, true);
        entityManager.setSignature(entityID, signature);

        // Notify systems that this entity’s signature changed
        systemManager.entitySignatureChanged(entityID, signature);
    }

    template<typename T>
    void removeComponent(EntityID entity)
    {
        componentManager.removeComponent<T>(entity);

        ComponentID compID = componentManager.getComponentType<T>();
        Signature signature = entityManager.getSignature(entity);
        signature.set(compID, false);
        entityManager.setSignature(entity, signature);

        systemManager.entitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& getComponent(EntityID entity) {
        return componentManager.getComponentArray<T>().getData(entity);
    }

    // System register, signature assignment
    template<typename T>
    shared_ptr<T> registerSystem()
    {
        return systemManager.registerSystem<T>();
    }

    template<typename T>
    void setSystemSignature(Signature signature)
    {
        systemManager.setSystemSignature<T>(signature);
    }

    template<typename T>
    ComponentID getComponentID() {
        return componentManager.getComponentType<T>();
    }

    // Update each time frame
    void update(float deltaTime);
    
    // Calling UI function
    void setState(std::unique_ptr<GameState> state) {
        std::cout << "[World] Switching state to: " << typeid(*state).name() << '\n';
        if (currentState)
            currentState->onExit(*this);

        currentState = std::move(state);

        if (currentState)
            currentState->onEnter(*this);
    }

    void handleEvent(sf::Event& event) {
        if (currentState) currentState->handleEvent(*this, event);
    }

    void render() {
        if (currentState) currentState->render(*this, window);
    }

    const std::unique_ptr<GameState>& getCurrentState() const {
        return currentState;
    }

    template<typename T>
    inline shared_ptr<T> getSystem() {
        return systemManager.getSystem<T>();
    }

    EntityManager getEntityManager();
    SystemManager getSystemManager();

    void requestClose() { shouldClose = true; }
    bool isRunning() const { return window.isOpen() && !shouldClose; }

};

