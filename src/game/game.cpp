#include "game.hpp"
#include "globals.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../utilities/randomizer.hpp"
#include "../states/gameplay/gameState.hpp"

void game::initializeWindow()
    {
        globals::_logger->logToConsole("Initializng window");
        
        app = new sf::RenderWindow(sf::VideoMode(1170, 700), "Breakout", sf::Style::Close);
        app->setFramerateLimit(60.0f);
    }

void game::initializeControls()
    {
        globals::_logger->logToConsole("Initializng controls");

        globals::_mouseManager->add("editor_left_mouse_press",      sf::Mouse::Left,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_mouseManager->add("editor_right_mouse_press",     sf::Mouse::Right,       inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);


        globals::_keyboardManager->add("editor_spin_block_right",   sf::Keyboard::E,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_spin_block_left",    sf::Keyboard::Q,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_delete_entity",      sf::Keyboard::Delete,   inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_snap_to_grid",       sf::Keyboard::LAlt,     inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
		globals::_keyboardManager->add("editor_toggle_resize",		sf::Keyboard::R,		inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_move_view_right",    sf::Keyboard::D,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_left",     sf::Keyboard::A,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_up",       sf::Keyboard::W,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_move_view_down",     sf::Keyboard::S,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_zoom_view_out",      sf::Keyboard::Q,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_zoom_view_in",       sf::Keyboard::E,        inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);


        globals::_keyboardManager->add("editor_save_level",         sf::Keyboard::F3,       inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        globals::_keyboardManager->add("editor_load_level",         sf::Keyboard::F4,       inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);

        globals::_keyboardManager->add("editor_confirm",            sf::Keyboard::Return,   inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);
        

        globals::_keyboardManager->add("change_to_editor",          sf::Keyboard::F1,       inputState::ON_PRESS,			states::GAME_STATE);
        globals::_keyboardManager->add("change_to_game",            sf::Keyboard::F2, [] () 
                                                                    { globals::_stateMachine->popState(); }, 
                                                                                            inputState::ON_PRESS,			states::LEVEL_EDITOR_STATE);

    }

void game::initialize()
    {
#if _DEBUG
        _logger = new logr::logger("defaultLogger", "log", false);
        _logger->clear();
    #else
        _logger = new logr::logger("defaultLogger", "log", true);
#endif
        globals::_scriptManager =   &_scriptManager;

        globals::_keyboardManager = &_keyboardManager;
        globals::_mouseManager =    &_mouseManager;

        globals::_textEntered =     &_textEntered;

        globals::_eventManager =    &_eventManager;
        globals::_stateMachine =    &_stateMachine;
        globals::_logger =          _logger;


        globals::_logger->logToConsole("Initializing game");

        rndm::initRandom();

        initializeWindow();
        initializeControls();

        globals::_stateMachine->setWindow(app);
        globals::_stateMachine->queueState(new gameState);
        
    }

void game::cleanup()
    {
        if (_logger)
            {
                globals::_logger->logToConsole("Cleaning game loop");
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
                globals::_logger->logToConsole("Freeing Logger Memory. Does this mean I'm dead?");
                delete _logger;
                _logger = nullptr;
            }

        globals::_scriptManager = nullptr;
        globals::_eventManager = nullptr;
        globals::_keyboardManager = nullptr;
        globals::_logger = nullptr;
        globals::_mouseManager = nullptr;
        globals::_stateMachine = nullptr;
    }

void game::start()
    {
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

                globals::_stateMachine->handleInput();
                while (accumulator >= updateTime)
                    {
                        globals::_stateMachine->tick(deltaTime);
                        accumulator -= updateTime;
                    }
                globals::_stateMachine->render();

				_stateMachine.setDeltaTime(deltaTime.asSeconds());
            }
    }

game::~game()
    {}
