// globals.hpp
// all global variables needed
#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../states/gameStates/states.hpp"

class console;

template<typename T>
class inputManager;

class stringInputManager;

class stateMachine;
class eventManager;

class scriptManager;

namespace logr
    {
        class logger;
    }

class globals
    {
        public:
            static console *_console;

            static scriptManager *_scriptManager;

            static inputManager<sf::Keyboard::Key> *_keyboardManager;
            static inputManager<sf::Mouse::Button> *_mouseManager;

            static stringInputManager *_textEntered;

            static stateMachine *_stateMachine;
            static eventManager *_eventManager;
            static logr::logger *_logger;

            static states _gameStates;

    };