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

#include "menuState.hpp"

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
		_uiFactory.setFont("assets/textures/fonts/Squares_Bold_Free.otf");

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

		_uiFactory.add("lives", "Lives Left:");
		_uiFactory.add("livesLeft", std::to_string(_lives));

		_uiFactory.add("gameOverText", "");
		_uiFactory.getText("gameOverText")->setColor(sf::Color::Red);

		_gameOver = false;
    }

void gameState::render()
    {
        _world.draw(*globals::_stateMachine->getWindow());
		_uiFactory.draw(*globals::_stateMachine->getWindow());
    }

void gameState::update(sf::Time deltaTime)
    {
		auto lives = _uiFactory.getText("lives");
		auto livesLeft = _uiFactory.getText("livesLeft");
		auto gameOverText = _uiFactory.getText("gameOverText");
		auto windowView = globals::_stateMachine->getWindow()->getView();

		lives->setPosition(windowView.getCenter() - (windowView.getSize() / 2.f));
		livesLeft->setPosition(windowView.getCenter() -
							  (windowView.getSize() / 2.f) +
				 sf::Vector2f((lives->getCharacterSize() * lives->getString().getSize()) / 1.5, 0));

		gameOverText->setPosition(windowView.getCenter() - 
			         sf::Vector2f(gameOverText->getGlobalBounds().width / 2, gameOverText->getGlobalBounds().height / 2.f));
        
		if (_nextLevel)
            {
                if (_levelList.size() > (_currentLevel + 1))
                    {
                        _world.load(_levelList[++_currentLevel]);
                    }
                else
                    {
						if (!_gameOver)
							{
								_endGameCountdown.start(sf::seconds(4));
							}
						_gameOver = true;
						_uiFactory.getText("gameOverText")->setString("You have completed all of the levels. You Win!\n" +
							                           std::to_string(static_cast<int>(_endGameCountdown.getRemainingTime().asSeconds())));
                    }
                _nextLevel = false;
            }

        if (_lostLive)
            {
                _lostLive = false;
				_uiFactory.getText("livesLeft")->setString(std::to_string(_lives));
            }

		if (_lives <= 0)
            {
				if (!_gameOver) 
					{
						_endGameCountdown.start(sf::seconds(4));
					}
				_gameOver = true;
				_uiFactory.getText("gameOverText")->setString("You lost all of your Lives. Game Over\n" +
									           std::to_string(static_cast<int>(_endGameCountdown.getRemainingTime().asSeconds())));
            }

		if (_gameOver && _endGameCountdown.isDone())
			{
				globals::_stateMachine->popState();
				globals::_stateMachine->queueState(new menuState);
			}

		if (!_endGameCountdown.getRunning()) 
			{
				_world.update(deltaTime);
			}
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
