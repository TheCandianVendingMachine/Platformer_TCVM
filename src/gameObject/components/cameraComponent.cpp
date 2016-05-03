#include "cameraComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../gameObject/gameObject.hpp"
#include "components.hpp"

void cameraComponent::setWindow(sf::RenderWindow *app)
	{
		_app = app;
	}

void cameraComponent::setFollowRadius(float radius)
    {
        _followRadius = radius;
    }

void cameraComponent::setCameraOffset(sf::Vector2f offset)
	{
        _offset = offset;
	}

void cameraComponent::setCameraSize(sf::Vector2f size)
	{
        _view.setSize(size);
	}

void cameraComponent::setFollow(bool follow)
	{
		_follow = follow;
        auto tc = _obj->get<textureComponent>();
        if (tc)
            {
                _view.setCenter(tc->getSprite()->getPosition());
            }
	}

void cameraComponent::update(sf::Time deltaTime)
	{
		if (_follow)
			{
				auto tc = _obj->get<textureComponent>();
				if (tc)
					{
                        sf::Vector2f objPos = tc->getSprite()->getPosition();
						bool outOfRadiusXMin = (objPos.x + _offset.x) < _view.getCenter().x - _followRadius;
						bool outOfRadiusXMax = (objPos.x + _offset.x) > _view.getCenter().x + _followRadius;
						bool outOfRadiusYMin = (objPos.y + _offset.y) < _view.getCenter().y - _followRadius;
						bool outOfRadiusYMax = (objPos.y + _offset.y) > _view.getCenter().y + _followRadius;
                        if (outOfRadiusXMax || outOfRadiusXMin || outOfRadiusYMax || outOfRadiusYMin)
                            {
								float speed = 750.f;
								auto mc = _obj->get<movementComponent>();
								if (mc)
									{
										// Set the speed of camera 100 speeds more than hte follow objects max speed
										// This is so the follow object can never leave the camera view, and if he does
										// he will return to the view eventually
										speed = mc->getMaxSpeed() + 100.f;
									}
								float angle = atan2(objPos.y - _view.getCenter().y, objPos.x - _view.getCenter().x);
								_impulse = sf::Vector2f(cos(angle), sin(angle)) * speed;
                            }
						else
							{
								_impulse = sf::Vector2f(0, 0);
							}
					}

				_view.move(_impulse * deltaTime.asSeconds());
				_app->setView(_view);
			}
	}
