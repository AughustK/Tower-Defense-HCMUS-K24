#pragma once

#include "System.h"
#include "EntityManager.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <cassert>

using namespace std;

class SystemManager
{
private:
	unordered_map<const char*, shared_ptr<System>> m_systems{};
	unordered_map<const char*, vector<shared_ptr<System>>> m_signatures{};
	vector<weak_ptr<System>> m_orderedSystems{};

public:
	template<typename T>
	shared_ptr<T> RegisterSystem();

	template<typename T>
	void setSystemSignature(Signature signature);

	template<typename T>
	shared_ptr<T> getSystem() const;
};


