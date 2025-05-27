#pragma once

#include "World.h"

#include <vector>
#include <memory>
#include <cstdint>
using namespace std;

class System
{
protected:
		World* world = nullptr;
		bool active = true;

public:
	// Prevent Accidental Copies
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	System(System&&) = delete;
	System& operator=(System&&) = delete;

	virtual ~System() = default;

	virtual void initalize(World* tempWorld) 
	{
		world = tempWorld;
	}

	// Pure virtual: each derived system (must update) every frame.
	// deltaTime is time elapsed (in seconds) since the last call.
	virtual void update(float deltaTime) = 0;
};

