// gameState.hpp
// manages the game state. Has the world, player, etc inside as members
#pragma once

#include "../state.hpp"
#include "gameworld.hpp"

class gameState : public state
    {
        private:
            gameworld _world;

        public:
            gameState();

            void initialize();

            void render();
            void update(sf::Time deltaTime);

            void cleanup();

            ~gameState();

    };