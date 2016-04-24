#include "physicsComponent.hpp"
#include "movementComponent.hpp"
#include "collisionComponent.hpp"
#include "../gameObject/gameObject.hpp"

void physicsComponent::setGravity(float gravity)
    {
        _gravity = gravity;
    }

void physicsComponent::setTerminalVelocity(float terminalVelocity)
    {
        _terminalVelocity = terminalVelocity;
    }

void physicsComponent::update(sf::Time deltaTime)
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                sf::Vector2f impulse(0, 0);
                auto cc = _obj->get<collisionComponent>();

                if (cc)
                    {
                        if (!cc->getColliding())
                            {
                                impulse = sf::Vector2f(0, mc->getImpulse().y + (_gravity * deltaTime.asSeconds()));
                                if (impulse.y >= _terminalVelocity)
                                    {
                                        impulse.y = _terminalVelocity;
                                    }
                            }
                    }
                else
                    {
                        impulse = sf::Vector2f(0, mc->getImpulse().y + (_gravity * deltaTime.asSeconds()));
                        if (impulse.y >= _terminalVelocity)
                            {
                                impulse.y = _terminalVelocity;
                            }
                    }

                mc->move(impulse);
            }
    }
