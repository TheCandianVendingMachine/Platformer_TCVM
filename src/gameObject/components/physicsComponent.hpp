// physicsComponent.hpp
// allows the entity to experience physics
#pragma once

#include "component.hpp"

#include <SFML/System/Vector2.hpp>

class gameObject;

class physicsComponent : public component
    {
        private:
            float _gravity;
            float _terminalVelocity;
            float _frictionCoefficient;
            
        public:
            void setGravity(float gravity);
            void setTerminalVelocity(float terminalVelocity);
            void setFriction(float friction);

            void update(sf::Time deltaTime);

    };