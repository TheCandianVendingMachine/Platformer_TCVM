#include "collisionComponent.hpp"
#include "textureComponent.hpp"

#include "../gameObject/gameObject.hpp"

std::pair<sf::Vector2f, sf::Vector2f> collisionComponent::_getDistance(collisionComponent &other)
    {
        auto first = *this;
        auto second = other;

        auto firstObjBound = first.getBounds();
        auto secondObjBound = second.getBounds();

        sf::Vector2f centerFirst(firstObjBound.left + (firstObjBound.width / 2),
                                 firstObjBound.top + (firstObjBound.height / 2));

        sf::Vector2f centerSecond(secondObjBound.left + (secondObjBound.width / 2),
                                  secondObjBound.top + (secondObjBound.height / 2));

        sf::Vector2f distance(centerFirst - centerSecond);
        sf::Vector2f minDistance((firstObjBound.width / 2) + (secondObjBound.width / 2),
                                 (firstObjBound.height / 2) + (secondObjBound.height / 2));

        return std::make_pair(distance, minDistance);
    }

sf::Vector2f collisionComponent::_getOverlap(collisionComponent &other)
    {
        auto distance = _getDistance(other);
        return sf::Vector2f(distance.first.x > 0 ? distance.second.x - distance.first.x : -distance.second.x - distance.first.x,
                            distance.first.y > 0 ? distance.second.y - distance.first.y : -distance.second.y - distance.first.y);
    }

sf::FloatRect &collisionComponent::getBounds()
    {
        return _boundingBox;
    }

bool collisionComponent::getColliding()
    {
        return _colliding;
    }

bool collisionComponent::hasCollided(collisionComponent &other)
    {
        auto distance = _getDistance(other);
        _colliding = abs(distance.first.x) < distance.second.x && abs(distance.first.y) < distance.second.y;
        return _colliding;
    }

bool collisionComponent::collide(collisionComponent &other)
    {
        if (hasCollided(other))
            {
                auto objSprite = _obj->get<textureComponent>();
                if (objSprite)
                    {
                        auto obj = objSprite->getSprite();
                        sf::Vector2f overlap = _getOverlap(other);
                        if (abs(overlap.x) > abs(overlap.y))
                            {
                                // if Y is shallow axis, the entity will move to the bottom/top of the object
                                obj->setPosition(obj->getPosition().x, obj->getPosition().y + overlap.y);
                            }
                        else if (abs(overlap.x) < abs(overlap.y))
                            {
                                // if X is shallow axis, the entity will move to the side of the object
                                obj->setPosition(obj->getPosition().x + overlap.x, obj->getPosition().y);
                            }
                        else
                            {
                                // equal on both sides. Entity will move in diagonal direction
                               obj->setPosition(obj->getPosition().x + overlap.x, obj->getPosition().y + overlap.y);
                            }

                        update();
                        return true;
                    }
            }

        return false;
    }

collisionComponent::collisionComponent()
    {
        _colliding = false;
        _boundingBox = sf::FloatRect();
        _positionOffset = sf::Vector2f();
    }

void collisionComponent::setBounds(sf::Vector2f size, sf::Vector2f offset)
    {
        _boundingBox.width = size.x;
        _boundingBox.height = size.y;

        _positionOffset = offset;
    }

void collisionComponent::update()
    {
        auto objSprite = _obj->get<textureComponent>();
        if (objSprite)
            {
                sf::Vector2f objPos = objSprite->getSprite()->getPosition();
                _boundingBox.left = objPos.x + _positionOffset.x;
                _boundingBox.top = objPos.y + _positionOffset.y;
            }
    }