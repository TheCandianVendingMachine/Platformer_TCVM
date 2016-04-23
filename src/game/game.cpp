#include "game.hpp"
#include "globals.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../utilities/randomizer.hpp"
#include "../states/gameplay/gameState.hpp"

void game::initializeWindow()
    {
        globals::_logger->log("Initializng window");
        
        app = new sf::RenderWindow(sf::VideoMode(1170, 700), "Breakout", sf::Style::Close);
        app->setFramerateLimit(60.0f);
    }

void game::initializeSounds()
    {
        globals::_logger->log("Initializng sounds");
    }

void game::initializeTextures()
    {
        globals::_logger->log("Initializng textures");

        globals::_textureManager->add("assets/textures/platform.png", "platformTexture");
        globals::_textureManager->add("assets/textures/player.png", "playerTexture");
    }

void game::initializeControls()
    {
        globals::_logger->log("Initializng controls");

        globals::_mouseManager->add("editor_left_mouse_press", sf::Mouse::Left, true, LEVEL_EDITOR_STATE);
        globals::_mouseManager->add("editor_right_mouse_press", sf::Mouse::Right, true, LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_spin_block_right", sf::Keyboard::E, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_spin_block_left", sf::Keyboard::Q, true, LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_delete_entity", sf::Keyboard::Delete, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_snap_to_grid", sf::Keyboard::LAlt, true, LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_move_view_right", sf::Keyboard::D, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_left", sf::Keyboard::A, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_up", sf::Keyboard::W, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_down", sf::Keyboard::S, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_zoom_view_out", sf::Keyboard::Q, true, LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_zoom_view_in", sf::Keyboard::E, true, LEVEL_EDITOR_STATE);
        
        globals::_keyboardManager->add("change_to_editor", sf::Keyboard::F1, true, GAME_STATE);
        globals::_keyboardManager->add("change_to_game", sf::Keyboard::F2, [] () { globals::_stateMachine->popState(); }, true, LEVEL_EDITOR_STATE);


        globals::_keyboardManager->add("player_jump", sf::Keyboard::Space, true, GAME_STATE);
        globals::_keyboardManager->add("player_move_left", sf::Keyboard::Left, true, GAME_STATE);
        globals::_keyboardManager->add("player_move_right", sf::Keyboard::Right, true, GAME_STATE);
    }

void game::initialize()
    {
#if _DEBUG
        _logger = new logr::logger("defaultLogger", "log", false);
    #else
        _logger = new logr::logger("defaultLogger", "log", true);
#endif
        globals::_textureManager =  &_textureManager;
        globals::_fontManager =     &_fontManager;

        globals::_keyboardManager = &_keyboardManager;
        globals::_mouseManager =    &_mouseManager;

        globals::_eventManager =    &_eventManager;
        globals::_stateMachine =    &_stateMachine;
        globals::_logger =          _logger;

        globals::_logger->log("Initializng game");

        rndm::initRandom();

        initializeWindow();
        initializeTextures();
        initializeSounds();
        initializeControls();

        globals::_stateMachine->setWindow(app);
        globals::_stateMachine->queueState(new gameState);
        
    }

void game::cleanup()
    {
        if (_logger)
            {
                globals::_logger->log("Cleaning game loop");
            }

        if (app)
            {
                delete app;
                app = nullptr;
            }

        if (globals::_stateMachine->getAllStates()->empty())
            {
                for (auto &state : *globals::_stateMachine->getAllStates())
                    {
                        globals::_stateMachine->popState();
                    }
            }

        globals::_stateMachine->cleanup();

        if (_logger)
            {
                globals::_logger->log("Freeing Logger Memory. Does this mean I'm dead?");
                delete _logger;
                _logger = nullptr;
            }

        globals::_eventManager = nullptr;
        globals::_fontManager = nullptr;
        globals::_keyboardManager = nullptr;
        globals::_logger = nullptr;
        globals::_mouseManager = nullptr;
        globals::_stateMachine = nullptr;
        globals::_textureManager = nullptr;
    }

void game::start()
    {
        bool updateOnFocus = false;

        initialize();

        sf::Clock deltaClock; 
        deltaClock.restart();

        float updateTime = 1.f / 60.f;

        sf::Time currentTime = deltaClock.getElapsedTime();
        float accumulator = 0.0f;

        while (app->isOpen())
            {
                sf::Time newTime = deltaClock.getElapsedTime();
                sf::Time deltaTime = newTime - currentTime;
                currentTime = newTime;

                accumulator += deltaTime.asSeconds();

                while (accumulator >= updateTime)
                    {
                        if (app->hasFocus() && !updateOnFocus)
                            {
                                globals::_stateMachine->tick(deltaTime);
                            }
                        accumulator -= updateTime;
                    }
                globals::_stateMachine->render();
            }
    }

game::~game()
    {}
