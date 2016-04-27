#include "gameState.hpp"

#include "../../game/globals.hpp"

#include "../levelEditor.hpp"

#include "../../gameObject/gameObject/gameObject.hpp"
#include "../../gameObject/components/textureComponent.hpp"

#include <logger.hpp>
#include "../../managers/input/inputManager.hpp"
#include "../stateMachine.hpp"

gameState::gameState()
    {
        _renderOvertop = false;
        _updateUnderneath = false;
        _state = GAME_STATE;
    }

void gameState::initialize()
    {
        globals::_logger->logToConsole("Initializing game state");
        globals::_keyboardManager->changeFunction("change_to_editor", [this] () { globals::_stateMachine->queueState(new levelEditor(&_world)); });
    }

void gameState::render()
    {
        _world.draw(*globals::_stateMachine->getWindow());
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
