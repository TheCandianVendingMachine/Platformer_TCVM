// globals.hpp
// all global variables needed
#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <logger.hpp>

#include "../states/states.hpp"
#include "../states/stateMachine.hpp"
#include "../managers/events/eventManager.hpp"
#include "../managers/input/inputManager.hpp"
#include "../managers/resourceManager.hpp"

class globals
    {
        public:
            static inputManager<sf::Keyboard::Key> *_keyboardManager;
            static inputManager<sf::Mouse::Button> *_mouseManager;

            static resourceManager<sf::Font> *_fontManager;
            static resourceManager<sf::Texture> *_textureManager;

            static stateMachine *_stateMachine;
            static eventManager *_eventManager;
            static logr::logger *_logger;

            static states _gameStates;

    };