#include "gameState.hpp"
#include "../../game/globals.hpp"
#include "../levelEditor.hpp"

#include "../../gameObject/gameObject/gameObject.hpp"

#include "../../gameObject/components/textureComponent.hpp"

gameState::gameState()
    {
        _renderOvertop = false;
        _updateUnderneath = false;
        _state = GAME_STATE;
    }

void gameState::initialize()
    {
        globals::_logger->logToConsole("Initializing game state");
        //globals::_keyboardManager->changeFunction("change_to_editor", [this] () { globals::_stateMachine->queueState(new levelEditor(&_world)); });

        _world.addEntity("player");
        auto it = _world.addEntity("platform");
        auto tc = it->get<textureComponent>();
        if (tc)
            {
                tc->setPosition(0, 690);
            }

        it = _world.addEntity("platform");
        tc = it->get<textureComponent>();
        if (tc)
            {
                tc->setPosition(0, 0);
            }
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
