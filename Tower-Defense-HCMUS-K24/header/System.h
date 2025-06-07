#pragma once

#include <bitset>
#include <vector>
#include <algorithm>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include "../header/Managers/EntityManager.h"

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
	inline shared_ptr<T> registerSystem();

	template<typename T>
	inline void setSystemSignature(Signature signature);

	void removeEntitySystem(EntityID entityID);

	void entitySignatureChanged(EntityID entityID, Signature entitySignature);

	const unordered_map<type_index, shared_ptr<System>>& getSystems() const;

};	



