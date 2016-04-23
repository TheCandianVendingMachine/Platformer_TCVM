// gameworld.hpp
// the games world. Holds the current level, and manages all entities on screen
#pragma once

#include <vector>
#include <SFML/System/Time.hpp>

#include "level.hpp"

#include "../../entity/player.hpp"

class entity;

class gameworld
    {
        private:
            std::vector<entity*> _entities;
            level _level;

            player _plr;

        public:
            gameworld();

            void draw();
            void update(sf::Time deltaTime);

            level *getLevel();

    };