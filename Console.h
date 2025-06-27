#pragma once
#include <SFML/Graphics.hpp>
#include "ChoosingMap.h"
#include "Credit.h"

using namespace std;
using namespace sf;

enum class AppState { Menu, ChoosingMap, Credit };

class Console {
    RenderWindow window;
    AppState currentState = AppState::Menu;
    Credit* creditScreen = nullptr; 
public:
    Console();
    void run();
};