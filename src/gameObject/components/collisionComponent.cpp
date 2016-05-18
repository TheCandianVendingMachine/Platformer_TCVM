#include "collisionComponent.hpp"
#include "textureComponent.hpp"
#include "stateComponent.hpp"

#include "../../game/globals.hpp"
#include "../../managers/scriptManager/scriptManager.hpp"

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

bool collisionComponent::collide(gameObject *other)
    {
		auto otherCC = other->get<collisionComponent>();
		if (otherCC)
			{
				if(!((otherCC->getBounds().left > (getBounds().left + (getBounds().width)) ||
					 (otherCC->getBounds().left + (otherCC->getBounds().width) < getBounds().left)) &&
                     (otherCC->getBounds().top > (getBounds().top + (getBounds().height)) ||
                     (otherCC->getBounds().top + (otherCC->getBounds().height) < getBounds().top))))
					{
						if (hasCollided(*otherCC))
							{
								auto objSprite = _obj->get<textureComponent>();
								if (objSprite)
									{
										auto obj = objSprite->getSprite();
										sf::Vector2f overlap = _getOverlap(*otherCC);

										if (!_onCollide.empty())
											{
												if (otherCC->getSurfaceType() == NON_COLLIDABLE)
													{
														overlap.x = 0.f;
														overlap.y = 0.f;
													}

												globals::_scriptManager->callLuaFunc(_onCollide,
													*_obj->getGameObjectHandle(), *other->getGameObjectHandle(),
													overlap.x, overlap.y);
											}

										update();
										_colliding = true;
									}
							}
					}
			}

		return _colliding;
    }

collisionComponent::collisionComponent()
    {
        _colliding = false;
        _boundingBox = sf::FloatRect();
        _positionOffset = sf::Vector2f();
    }

void collisionComponent::setOnCollisionScript(const std::string &collisionScript)
    {
        _onCollide = collisionScript;
    }

void collisionComponent::setBounds(sf::Vector2f size, sf::Vector2f offset)
    {
        _boundingBox.width = size.x;
        _boundingBox.height = size.y;

        _positionOffset = offset;
    }

sf::FloatRect collisionComponent::getBounds()
    {
        return _boundingBox;
    }

void collisionComponent::setSurfaceType(surfaceType type)
    {
        _surfaceType = type;
    }

collisionComponent::surfaceType collisionComponent::getSurfaceType()
    {
        return _surfaceType;
    }

void collisionComponent::update()
    {
        auto objSprite = _obj->get<textureComponent>();
        if (objSprite)
            {
                _boundingBox = objSprite->getSprite()->getGlobalBounds();
            }
    }
