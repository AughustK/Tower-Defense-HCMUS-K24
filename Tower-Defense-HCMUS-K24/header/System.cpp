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
				std::cout << "[SystemManager] Adding entity " << entityID
					<< " to system: " << typeName.name() << '\n';
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


