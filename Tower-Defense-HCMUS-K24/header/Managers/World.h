#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "../System.h"                  
#include <vector>
#include <memory>
#include <cassert>

class System;                        

class World
{
private:
    EntityManager    entityManager;       
    ComponentManager componentManager;    
    SystemManager systemManager;

public:
    World();
    ~World() = default;

    // Entity creation/destruction
    EntityID createEntity();
    void destroyEntity(EntityID entityID);


    // Component register, management
    template <typename T>
    void registerComponent();

    template <typename T>
    void addComponent(EntityID entityID, const T& component);

    template <typename T>
    void removeComponent(EntityID entityID);

    template <typename T>
    T& getComponent(EntityID entityID);


    // System register, signature assignment
    template<typename T>
    shared_ptr<T> registerSystem();

    template<typename T>
    void setSystemSignature(Signature signature);

    template<typename T>
    void removeEntitySystem(EntityID entityID);

    // Update each time frame
    void update(float deltaTime);
    
};

