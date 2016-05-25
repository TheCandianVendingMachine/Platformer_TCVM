#include "menuState.hpp"
#include "gameState.hpp"

#include "../game/globals.hpp"
#include "stateMachine.hpp"

#include "../UI/regular/clickableButton.hpp"

menuState::menuState()
    {
        _isInitialized = false;
        _state = MENU_STATE;

        _uiFactory.setFont("assets/textures/fonts/Squares_Bold_Free.otf");
    }

void menuState::render()
    {
        _uiFactory.draw(*globals::_stateMachine->getWindow());
    }

void menuState::update(sf::Time deltaTime)
    {
        _uiFactory.update();
    }

void menuState::initialize()
    {
        _isInitialized = true;
        auto app = globals::_stateMachine->getWindow();

        _start.getText()->setFont(*_uiFactory.getFont());
        _start.setString("Start Game");
        _start.setColour(sf::Color::Red);
        _start.setWindow(*app);
        _start.setPosition(sf::Vector2f((app->getSize().x / 2) - (_start.getText()->getLocalBounds().width / 2),
                          (app->getSize().y / 2) - (_start.getText()->getLocalBounds().height / 2) - 30));
        _start.setFunction([] () 
            {
                globals::_stateMachine->queueState(new gameState);
            });
        _uiFactory.add("start", &_start);

        _quit.getText()->setFont(*_uiFactory.getFont());
        _quit.setString("Quit Game");
        _quit.setColour(sf::Color::Red);
        _quit.setWindow(*app);
        _quit.setPosition(sf::Vector2f((app->getSize().x / 2) - (_quit.getText()->getLocalBounds().width / 2),
                          (app->getSize().y / 2) - (_quit.getText()->getLocalBounds().height / 2) + 30));
        _quit.setFunction([&app] () 
            {
                globals::_stateMachine->closeWindow();
            });
        _uiFactory.add("quit", &_quit);
    }

void menuState::cleanup()
    {
        _isInitialized = false;
    }

menuState::~menuState()
    {
    
    }
