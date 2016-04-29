// movementComponent.hpp
// allows for entity movement
#pragma once

#include "component.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class gameObject;

class movementComponent : public component
    {
        private:
            sf::Vector2f _impulse;

            float _maxSpeed;
            float _acceleration;

        public:
            sf::Vector2f getImpulse();

            void move(sf::Vector2f impulse);
            void update(sf::Time deltaTime);

            float getMaxSpeed();
            void setMaxSpeed(float speed);

            float getAcceleration();
            void setAcceleration(float acceleration);

    };