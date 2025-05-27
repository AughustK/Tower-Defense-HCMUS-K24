#pragma once
#include <cstddef>   // for size_t
using namespace std;

using ComponentTypeID = size_t; // Unique identifier for each component type

namespace ecs    
{
	inline ComponentTypeID g_nextComponentTypeID = 0;      
	template <typename C> 
	ComponentTypeID getComponentTypeID() noexcept
	{ 
		static ComponentTypeID typeID = g_nextComponentTypeID++;  // Assign a unique ID to each component type
		return typeID; 
	}
}
