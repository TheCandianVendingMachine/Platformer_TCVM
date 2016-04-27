#include "textureComponent.hpp"
#include "collisionComponent.hpp"
#include "../gameObject/gameObject.hpp"

void textureComponent::setTexture(sf::Texture texture)
    {
        _texture = texture;
        _sprite.setTexture(_texture);
    }

sf::Texture *textureComponent::getTexture()
    {
        return &_texture;
    }

sf::Sprite *textureComponent::getSprite()
    {
        return &_sprite;
    }

void textureComponent::setSize(float X, float Y)
	{
		_sprite.setScale(X / _size.x, X / _size.y);
		_size = sf::Vector2f(X, Y);
		auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
	}

void textureComponent::setSize(sf::Vector2f size)
	{
		_sprite.setScale(size.x / _size.x, size.y / _size.y);
		_size = size;
		auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
	}

sf::Vector2f textureComponent::getSize()
	{
		return _size;
	}

void textureComponent::setPosition(float X, float Y)
    {
        _sprite.setPosition(X, Y);
        auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
    }

void textureComponent::setPosition(sf::Vector2f pos)
    {
        _sprite.setPosition(pos);
        auto cc = _obj->get<collisionComponent>();
        if (cc)
            {
                cc->update();
            }
    }

void textureComponent::draw(sf::RenderWindow &app)
    {
        app.draw(_sprite);
    }
