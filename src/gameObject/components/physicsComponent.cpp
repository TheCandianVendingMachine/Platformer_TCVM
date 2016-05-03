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

void physicsComponent::setFriction(float friction)
    {
        _frictionCoefficient = friction;
    }

void physicsComponent::update(sf::Time deltaTime)
    {
        auto mc = _obj->get<movementComponent>();
        if (mc)
            {
                sf::Vector2f impulse(mc->getImpulse().x, 0);

                auto cc = _obj->get<collisionComponent>();
                if (cc)
                    {
                        if (!cc->getColliding())
                            {
                                impulse.y = mc->getImpulse().y + (_gravity * deltaTime.asSeconds());
                                if (impulse.y >= _terminalVelocity)
                                    {
                                        impulse.y = _terminalVelocity;
                                    }
                            }
						else
							{
								impulse.y = 0;
							}
                    }
                else
                    {
                        impulse.y = mc->getImpulse().y + (_gravity * deltaTime.asSeconds());
                        if (impulse.y >= _terminalVelocity)
                            {
                                impulse.y = _terminalVelocity;
                            }
                    }

                if (abs(impulse.x) > 0)
                    {
                        if (impulse.x > 0)
                            {
                                impulse.x -= _frictionCoefficient * deltaTime.asSeconds();
                                impulse.x < 0 ? impulse.x = 0 : 0;
                            }
                        else if (impulse.x < 0)
                            {
                                impulse.x += _frictionCoefficient * deltaTime.asSeconds();
                                // make sure we didnt over shoot with the friction
                                impulse.x > 0 ? impulse.x = 0 : 0;
                            }
                    }

                mc->move(impulse);
            }
    }
