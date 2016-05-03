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
                        if ((objPos.x > _view.getCenter().x + _followRadius || objPos.x < _view.getCenter().x - _followRadius) ||
                            (objPos.y > _view.getCenter().y + _followRadius || objPos.y < _view.getCenter().y - _followRadius))
                            {

                            }
					}
			}
	}
