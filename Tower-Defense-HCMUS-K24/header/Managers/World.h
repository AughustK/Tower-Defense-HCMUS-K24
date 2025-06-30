#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "../Systems/System.h"
#include "TransientArena.h"
#include "../GameStates/GameState.h"

class System;                        

class World
{
private:
    EntityManager    entityManager;       
    ComponentManager componentManager;    
    SystemManager systemManager;


    std::unique_ptr<GameState> currentState = nullptr;
    bool shouldClose = false;

public:
    World();
    ~World() = default;

    sf::RenderWindow window;
    static Arena transientArena;

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

    template <typename T>
    void addComponent(EntityID entityID, const T& component)
    {
        componentManager.addComponent<T>(entityID, component);

        // Update entity signature
        ComponentID compID = componentManager.getComponentType<T>();
        Signature signature = entityManager.getSignature(entityID);
        signature.set(compID, true);
        entityManager.setSignature(entityID, signature);

        // Notify systems of signature change
        systemManager.entitySignatureChanged(entityID, signature);
    }

    template <typename T>
    void removeComponent(EntityID entityID)
    {
        componentManager.removeComponent<T>(entityID);

        ComponentID compID = componentManager.getComponentType<T>();
        Signature signature = entityManager.getSignature(entityID);
        signature.set(compID, false);
        entityManager.setSignature(entityID, signature);

        systemManager.entitySignatureChanged(entityID, signature);
    }

    template<typename T>
    T& getComponent(EntityID entity) 
    {
        return componentManager.getComponentArray<T>().getData(entity);
    }

    template <typename T>
    vector<EntityID> getEntitiesWithComponent() 
    {
        return componentManager.getEntitiesWithComponent<T>();
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
    void removeEntitySystem(EntityID entityID)
    {
        auto sys = systemManager.getSystem<T>();
        sys->removeEntity(entityID);
    }

    template<typename T>
    ComponentID getComponentType() 
    {
        return componentManager.getComponentType<T>();
    }

    template<typename T>
    shared_ptr<T> getSystem() 
    {
        return systemManager.getSystem<T>();
    }

    void setState(std::unique_ptr<GameState> state);
    void handleEvent(sf::Event& event);
    void render();
    const std::unique_ptr<GameState>& getCurrentState() const;

    EntityManager& getEntityManager();
    SystemManager& getSystemManager();
    ComponentManager& getComponentManager();

    // Update each time frame
    void update(float deltaTime);
    void requestClose();
    bool isRunning() const;
    
};

