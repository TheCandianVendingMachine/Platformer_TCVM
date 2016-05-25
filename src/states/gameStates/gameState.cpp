#include "gameState.hpp"

#include "../game/globals.hpp"

#include "levelEditor.hpp"

#include "../gameObject/gameObject/gameObject.hpp"
#include "../gameObject/components/textureComponent.hpp"

#include "../utilities/logger/logger.hpp"
#include "../managers/input/inputManager.hpp"
#include "stateMachine.hpp"

#include "../managers/events/eventManager.hpp"
#include "../managers/events/events.hpp"

#include "../utilities/loadJsonFile.hpp"

#include <algorithm>

gameState::gameState()
    {
        _renderOvertop = false;
        _updateUnderneath = false;
        _state = GAME_STATE;

        _lives = 3;

        globals::_eventManager->subscribe(this, NEXT_LEVEL);
        globals::_eventManager->subscribe(this, PLAYER_LOSE_HEALTH);
    }

void gameState::initialize()
    {
        globals::_logger->logToConsole("Initializing game state");
        globals::_keyboardManager->changeFunction("change_to_editor", [this] () { globals::_stateMachine->queueState(new levelEditor(&_world)); });

        Json::Value root;
        ljf::loadJsonFile("assets/levels/level_list.json", &root);

        for (auto &member : root.getMemberNames())
            {
                _levelList.push_back(root[member].asString());
            }

        _currentLevel = 0;
        if (!_levelList.empty())
            {
                _world.load(_levelList[_currentLevel]);
            }
    }

void gameState::render()
    {
        _world.draw(*globals::_stateMachine->getWindow());
    }

void gameState::update(sf::Time deltaTime)
    {
        if (_nextLevel)
            {
                if (_levelList.size() > (_currentLevel + 1))
                    {
                        _world.load(_levelList[++_currentLevel]);
                    }
                else
                    {
                        _lives = 0;
                    }
                _nextLevel = false;
            }

        if (_lives <= 0)
            {
                globals::_stateMachine->getWindow()->setView(globals::_stateMachine->getWindow()->getDefaultView());
                globals::_stateMachine->popState();
            }

        if (_lostLive)
            {
                _lostLive = false;
            }

        _world.update(deltaTime);
    }

void gameState::cleanup()
    {
        globals::_logger->logToConsole("Cleaning up game state");
    }

void gameState::alert(eventData data)
    {
        switch (data._event)
            {
                case NEXT_LEVEL:
                    _nextLevel = true;
                    break;
                case PLAYER_LOSE_HEALTH:
                    if (!_lostLive)
                        {
                            _lostLive = true;
                            _lives--;
                        }
                    break;
                default:
                    break;
            }
    }

gameState::~gameState()
    {
        globals::_eventManager->unsubscribe(this, NEXT_LEVEL);
        globals::_eventManager->unsubscribe(this, PLAYER_LOSE_HEALTH);
    }
