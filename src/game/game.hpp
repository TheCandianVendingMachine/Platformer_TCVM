// game.hpp
// the game loop. Initializes the window and resources that will be used in the game
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <logger.hpp>

#include "../managers/input/inputManager.hpp"
#include "../managers/input/stringInputManager.hpp"
#include "../states/stateMachine.hpp"
#include "../managers/events/eventManager.hpp"

class game
    {
        private:
            inputManager<sf::Keyboard::Key> _keyboardManager;
            inputManager<sf::Mouse::Button> _mouseManager;

            stringInputManager _textEntered;

            stateMachine _stateMachine;
            eventManager _eventManager;

            logr::logger *_logger;

        private:
            sf::RenderWindow *app;

            void initializeWindow();
            void initializeControls();
            void initialize();

        public:
            void start();
            void cleanup();

            ~game();

    };