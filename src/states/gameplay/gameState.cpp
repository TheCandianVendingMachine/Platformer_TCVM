#include "gameState.hpp"
#include "../../game/globals.hpp"
#include "../levelEditor.hpp"

gameState::gameState()
    {
        _renderOvertop = false;
        _updateUnderneath = false;
        _state = GAME_STATE;
    }

void gameState::initialize()
    {
        globals::_logger->logToConsole("Initializing game state");
        globals::_keyboardManager->changeFunction("change_to_editor", [this] () { globals::_stateMachine->queueState(new levelEditor(_world.getLevel())); });
    }

void gameState::render()
    {
        _world.draw();
    }

void gameState::update(sf::Time deltaTime)
    {
        _world.update(deltaTime);
    }

void gameState::cleanup()
    {
        globals::_logger->logToConsole("Cleaning up game state");
    }

gameState::~gameState()
    {
    }
