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

                textureComp->getSprite()->move(_impulse * deltaTime.asSeconds());

                auto cc = _obj->get<collisionComponent>();
                if (cc)
                    {
                        cc->update();
                    }
            }
    }
