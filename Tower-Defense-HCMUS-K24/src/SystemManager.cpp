#include "SystemManager.h"

template<typename T>
inline shared_ptr<T> SystemManager::RegisterSystem()
{
	const char* typeName = typeid(T).name();
	assert(m_systems.find(typeName) == m_systems.end() && "System already registered!");

	auto system = make_shared<T>();
	m_systems.insert({typeName, system});
	m_orderedSystems.push_back(system);
	SortSytemsByPriority();

	return system;
}

template<typename T>
void SystemManager::setSystemSignature(Signature signature)
{
	const char* typeName = typeid(T).name();
	assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");
	m.signatures.insert({ typeName, signature });
}

template<typename T>
shared_ptr<T> SystemManager::getSystem() const
{
	const char* typeName = typeid(T).name();
	auto systemIter = m_systems.find(typeName);
	if (systemIter != m_systems.end())
	{
		return static_pointer_cast<T>(systemIter->second);
	}
	return nullptr;
}


