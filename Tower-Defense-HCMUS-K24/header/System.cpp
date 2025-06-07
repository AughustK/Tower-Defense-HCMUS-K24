#include "System.h"

void System::addEntity(EntityID entityID)
{
	entities.push_back(entityID);
}

void System::removeEntity(EntityID entityID)
{
	entities.erase(remove(entities.begin(), entities.end(), entityID), entities.end());
}

const vector<EntityID>& System::getEntity() const
{
	return entities;
}

void SystemManager::init(EntityManager* em)
{
	entityManager = em;
}

template<typename T>
inline shared_ptr<T> SystemManager::registerSystem()
{
	const type_index typeName = type_index(typeid(T));
	assert(systems.find(typeName.name()) == systems.end() && "System already registered.");
	auto system = make_shared<T>();
	systems.insert({ typeName.name(), system });
	return system;
}

template<typename T>
inline void SystemManager::setSystemSignature(Signature signature)
{
	auto typeName = type_index(typeid(T));
	assert(systems.count(typeName) == 0 && "Already registered");
	systems[typeName] = make_shared<T>();
}

void SystemManager::removeEntitySystem(EntityID entityID)
{
	for (auto& [typeName, system] : systems)
	{
		system->removeEntity(entityID);
	}
}

void SystemManager::entitySignatureChanged(EntityID entityID, Signature entitySignature)
{
	for (auto& [typeName, system] : systems)
	{
		if (auto it = signatures.find(typeName); it != signatures.end())
		{
			const Signature& systemSignature = it->second;
			if ((systemSignature & entitySignature) == systemSignature)
			{
				system->addEntity(entityID);
			}
			else {
				system->removeEntity(entityID);
			}
		}
	}
}

const unordered_map<type_index, shared_ptr<System>>& SystemManager::getSystems() const
{
	return systems;
}


