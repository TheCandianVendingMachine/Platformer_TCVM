// player.hpp
// the player entity
#pragma once

#include "entity.hpp"

class player : public entity
    {
        private:
            sf::Vector2f _impulse;

        public:
            player();
            void update(sf::Time deltaTime);
            void collide();

    };