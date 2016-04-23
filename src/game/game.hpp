// game.hpp
// the game loop. Initializes the window and resources that will be used in the game
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <logger.hpp>

#include "../managers/input/inputManager.hpp"
#include "../managers/resourceManager.hpp"
#include "../states/stateMachine.hpp"
#include "../managers/events/eventManager.hpp"

class game
    {
        private:
            inputManager<sf::Keyboard::Key> _keyboardManager;
            inputManager<sf::Mouse::Button> _mouseManager;

            resourceManager<sf::Font> _fontManager;
            resourceManager<sf::Texture> _textureManager;

            stateMachine _stateMachine;
            eventManager _eventManager;

            logr::logger _logger;

        private:
            sf::RenderWindow *app;

            void initializeWindow();
            void initializeSounds();
            void initializeTextures();
            void initializeControls();
            void initialize();

            void cleanup();

        public:
            void start();

            ~game();

    };