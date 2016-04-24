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
            
        public:
            void setGravity(float gravity);
            void setTerminalVelocity(float terminalVelocity);

            void update(sf::Time deltaTime);

    };