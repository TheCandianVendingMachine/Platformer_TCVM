#include "cameraComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../gameObject/gameObject.hpp"
#include "components.hpp"

void cameraComponent::setWindow(sf::RenderWindow *app)
	{
		_app = app;
	}

void cameraComponent::setCameraSize(sf::Vector2f size)
	{
		_view.setViewport(sf::FloatRect(sf::Vector2f(_view.getViewport().left, _view.getViewport().top), size));
	}

void cameraComponent::setCameraPos(sf::Vector2f pos)
	{
		_view.setViewport(sf::FloatRect(pos, _view.getSize()));
	}

void cameraComponent::setFollow(bool follow)
	{
		_follow = follow;
	}

void cameraComponent::update(sf::Time deltaTime)
	{
		if (_follow)
			{
				auto tc = _obj->get<textureComponent>();
				if (tc)
					{
						sf::Vector2f objPos = tc->getSprite()->getPosition();
						sf::Vector2f camPos(_view.getViewport().left, _view.getViewport().top);

						float angle = atan2(objPos.y - camPos.y, objPos.x - camPos.x);
						_impulse = sf::Vector2f(cos(angle), sin(angle));
					}

				_view.move(_impulse * deltaTime.asSeconds());
			}
	}
