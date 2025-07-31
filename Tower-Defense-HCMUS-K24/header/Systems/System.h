#pragma once

#include <bitset>
#include <vector>
#include <algorithm>
#include <memory>         // std::shared_ptr, std::make_shared
#include <unordered_map>  // std::unordered_map
#include <typeindex>      // std::type_index
#include <typeinfo>       // typeid, std::type_info
#include <cassert>        // assert
#include <iostream>

#include "../Managers/EntityManager.h"

using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::type_index;

using EntityID = uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;

class System
{
protected:
	vector<EntityID> entities;

public:
    virtual ~System() = default;

    virtual void update(float deltaTime) = 0;
    virtual void clear() { entities.clear(); }

	virtual void addEntity(EntityID entityID);
	virtual void removeEntity(EntityID entityID);
	const vector<EntityID>& getEntity() const;
};

class SystemManager
{
private: 
	unordered_map<type_index, shared_ptr<System>> systems;
	unordered_map<type_index, Signature> signatures;
	EntityManager* entityManager = nullptr;

public:
	void init(EntityManager* em);

	template<typename T>
	shared_ptr<T> registerSystem()
	{
		const type_index typeName = type_index(typeid(T));
		assert(systems.find(typeName) == systems.end() && "System already registered.");
		auto system = std::make_shared<T>();
		systems.insert({ typeName, system });

		std::cout << "[SystemManager] Registered system: " << typeid(T).name() << "\n";

		return system;
	}

	template<typename T>
	void setSystemSignature(Signature signature)
	{
		auto typeName = type_index(typeid(T));
		// make sure T was registered
		assert(systems.count(typeName) != 0 && "System not registered");
		signatures[typeName] = signature;
		std::cout << "[SystemManager] Signature set for system: " << typeid(T).name() << "\n";
	}

	template<typename T>
	shared_ptr<T> getSystem()
	{
		type_index type = type_index(typeid(T));
		assert(systems.count(type) && "System not registered!");
		return std::static_pointer_cast<T>(systems.at(type));
	}

	void removeEntitySystem(EntityID entityID);

	void entitySignatureChanged(EntityID entityID, Signature entitySignature);

	const unordered_map<type_index, shared_ptr<System>>& getSystems() const;

};	



