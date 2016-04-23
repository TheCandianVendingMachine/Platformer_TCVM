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
    {}

gameState::~gameState()
    {
        cleanup();
    }
