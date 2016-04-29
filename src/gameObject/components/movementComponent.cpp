#include "movementComponent.hpp"
#include "textureComponent.hpp"
#include "physicsComponent.hpp"
#include "collisionComponent.hpp"

#include "../gameObject/gameObject.hpp"

sf::Vector2f movementComponent::getImpulse()
    {
        return _impulse;
    }

void movementComponent::move(sf::Vector2f impulse)
    {
        _impulse = impulse;
    }

void movementComponent::update(sf::Time deltaTime)
    {
        auto textureComp = _obj->get<textureComponent>();
        if (textureComp)
            {
                auto physicsComp = _obj->get<physicsComponent>();
                if (physicsComp)
                    {
                        physicsComp->update(deltaTime);
                    }

                if (abs(_impulse.x) >= _maxSpeed)
                    {
                        _impulse.x > 0 ? _impulse.x = _maxSpeed : _impulse.x = -_maxSpeed;
                    }

                textureComp->getSprite()->move(_impulse * deltaTime.asSeconds());

                auto cc = _obj->get<collisionComponent>();
                if (cc)
                    {
                        cc->update();
                    }
            }
    }

float movementComponent::getMaxSpeed()
    {
        return _maxSpeed;
    }

void movementComponent::setMaxSpeed(float speed)
    {
        _maxSpeed = speed;
    }

float movementComponent::getAcceleration()
    {
        return _acceleration;
    }

void movementComponent::setAcceleration(float acceleration)
    {
        _acceleration = acceleration;
    }
