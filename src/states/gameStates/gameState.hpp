// gameState.hpp
// manages the game state. Has the world, player, etc inside as members
#pragma once

#include "state.hpp"
#include "level.hpp"

#include "../managers/events/observer.hpp"

#include <vector>
#include <string>

class gameState : public state, public observer
    {
        private:
            level _world;

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