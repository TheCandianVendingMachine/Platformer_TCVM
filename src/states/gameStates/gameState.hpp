// gameState.hpp
// manages the game state. Has the world, player, etc inside as members
#pragma once

#include "state.hpp"
#include "level.hpp"

#include "../utilities/countdown.hpp"

#include "../managers/events/observer.hpp"
#include "../UI/regular/userInterfaceFactory.hpp"

#include <vector>
#include <string>

class gameState : public state, public observer
    {
		private:
			userInterfaceFactory _uiFactory;
            level _world;

			bool _gameOver;
			countdown _endGameCountdown;

            unsigned int _currentLevel;
            std::vector<std::string> _levelList;

            bool _nextLevel;

            bool _lostLive;
            int _lives;

        public:
            gameState();

            void initialize();

            void render();
            void update(sf::Time deltaTime);

            void cleanup();

            void alert(eventData data);

            ~gameState();

    };