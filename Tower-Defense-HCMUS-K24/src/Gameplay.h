#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "EntityManager.h"
#include <vector>
#include <memory>
using namespace std;

class Gameplay
{
private:


public: 
	Gameplay();
	~Gameplay();

	Entity m_enemy;
	Entity m_heroes;

};

#endif // GAMEPLAY_H

