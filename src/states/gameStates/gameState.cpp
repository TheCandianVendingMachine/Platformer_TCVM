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

        globals::_eventManager->subscribe(this, NEXT_LEVEL);
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
                // Because the Json file reads the file and pushed to the stack, we need to reverse it
                std::reverse(_levelList.begin(), _levelList.end());
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
                _world.unloadLevel();
                if (_levelList.size() > (_currentLevel + 1))
                    {
                        _world.load(_levelList[++_currentLevel]);
                    }
                _nextLevel = false;
            }
        _world.update(deltaTime);
    }

void gameState::cleanup()
    {
        globals::_logger->logToConsole("Cleaning up game state");
    }

void gameState::alert(eventData data)
    {
        _nextLevel = true;
    }

gameState::~gameState()
    {
        globals::_eventManager->unsubscribe(this, NEXT_LEVEL);
    }
