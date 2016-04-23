// player.hpp
// the player entity
#pragma once

#include "entity.hpp"

class player : public entity
    {
        private:
            sf::Vector2f _impulse;

            bool _enablePhysics;
            bool _jumping;

            const float _acceleration;
            const float _maxVelocity;

            const float _gravity;
            const float _terminalVelocity;

        public:
            player();
            void update(sf::Time deltaTime);

            void onCollide();
            void offCollide();

    };